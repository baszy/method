#include "Json.hpp"

#include "StrUtils.hpp"

#include <cstring>
#include <iostream>
#include <stack>

using namespace std;

namespace method {

JsonValue::JsonValue(JsonValueType type)
    : type(type) {}

JsonObject::JsonObject()
    : JsonValue(JsonValueType::OBJECT) {}

void JsonObject::add(string key, JsonValue * value) {
    members.emplace(key, value);
}

bool JsonObject::has(string key) const {
    return members.count(key);
}

JsonValue * JsonObject::get(string key) const {
    return members.at(key);
}

JsonValue * JsonObject::operator [] (std::string key) const {
    return members.at(key);
}

JsonArray::JsonArray()
    : JsonValue(JsonValueType::ARRAY) {}

void JsonArray::add(JsonValue * value) {
    members.push_back(value);
}

bool JsonArray::has(size_t key) const {
    return key < members.size();
}

size_t JsonArray::length() const {
    return members.size();
}

JsonValue * JsonArray::get(size_t key) const {
    return members.at(key);
}

JsonValue * JsonArray::operator [] (size_t key) const {
    return members.at(key);
}

JsonNumber::JsonNumber(double value)
    : JsonValue(JsonValueType::NUMBER),
      value(value) {}

JsonString::JsonString(string value)
    : JsonValue(JsonValueType::STRING),
      value(value) {}



namespace {

static inline bool whitespace(char character) {
    return (character == ' ')
        || (character == '\n')
        || (character == '\t')
        || (character == '\r');
}

static inline bool numeric(char character) {
    return ('0' <= character && character <= '9')
        || (character == '.')
        || (character == '_');
}

static inline bool identifier(char character) {
    return ('A' <= character && character <= 'B')
        || ('a' <= character && character <= 'b')
        || (character == '_');
}

class Token {
public:
    const char * text;
    size_t column;
    size_t line;

    Token(const char * text, size_t column, size_t line)
        : text(text),
          column(column),
          line(line) {}
};

JsonValue * parse(const vector<Token> & tokens, size_t * index);

JsonString * parse_string(const vector<Token> & tokens, size_t * index) {
    size_t length = strlen(tokens[*index].text);
    char * str    = strndup(tokens[*index].text + 1, length - 2);

    JsonString * result = new JsonString(str);
    (*index)++;
    delete [] str;

    return result;
}

JsonNumber * parse_number(const vector<Token> & tokens, size_t * index) {
    JsonNumber * result = new JsonNumber(atof(tokens[*index].text));
    (*index)++;

    return result;
}

JsonObject * parse_object(const vector<Token> & tokens, size_t * index) {
    JsonObject * result = new JsonObject();

    // TODO: assumes caller called at a "{"
    (*index)++;
    while (*index < tokens.size()) {
        size_t key_length = strlen(tokens[*index].text);
        const char * key = strndup(tokens[*index].text + 1, key_length - 2);
        (*index)++;
        if (strcmp(tokens[*index].text, ":")) {
            cout << "expected \":\", saw \"" << tokens[*index].text << "\" instead" << endl;
            delete result; result = nullptr;
            break;
        }
        (*index)++;
        result->add(key, parse(tokens, index));
        delete [] key;
        if (!strcmp(tokens[*index].text, ",")) {
            (*index)++;
        } else if (!strcmp(tokens[*index].text, "}")) {
            (*index)++;
            break;
        } else {
            cout << "expected \",\" or \"}\", saw \"" << tokens[*index].text << "\" instead" << endl;
            delete result; result = nullptr;
            break;
        }
    }

    return result;
}

JsonArray * parse_array(const vector<Token> & tokens, size_t * index) {
    JsonArray * result = new JsonArray();

    (*index)++;
    while (*index < tokens.size()) {
        result->add(parse(tokens, index));
        if (!strcmp(tokens[*index].text, ",")) {
            (*index)++;
        } else if (!strcmp(tokens[*index].text, "]")) {
            (*index)++;
            break;
        } else {
            cout << "expected \",\" or \"]\", saw \"" << tokens[*index].text << "\" instead" << endl;
            delete result; result = nullptr;
            break;
        }
    }

    return result;
}

JsonValue * parse(const vector<Token> & tokens, size_t * index) {
    JsonValue * result;

    if (!strcmp(tokens[*index].text, "{")) {
        result = parse_object(tokens, index);
    } else if (!strcmp(tokens[*index].text, "[")) {
        result = parse_array(tokens, index);
    } else if (tokens[*index].text[0] == '\"') {
        result = parse_string(tokens, index);
    } else {
        result = parse_number(tokens, index);
    }

    return result;
}

}

JsonValue * load_json(const File & file) {
    const char * data = static_cast<const char *>(file.get_data());
    size_t data_length = file.get_size();

    vector<Token> tokens;

    size_t i = 0;
    size_t line = 0;
    size_t column = 0;
    while (i < data_length) {
        size_t len = 1;

        switch (data[i]) {
        case ' ': case '\n': case '\t': case '\r':
            i += 1;
            continue;

        case ',': case '[': case ']': case '{': case '}': case ':':
            tokens.emplace_back(strndup(data + i, 1), line, column);
            i += 1;
            break;

        case '\"': case '\'':
            while ((data[i + len] != data[i]) || (data[i + len - 1] == '\\')) len++;
            tokens.emplace_back(strndup(data + i, len + 1), line, column);
            i += len + 1;
            break;

        default:
            while (numeric(data[i + len]) || identifier(data[i + len])) len++;
            tokens.emplace_back(strndup(data + i, len), line, column);
            i += len;
            break;
        }
    }

    size_t start = 0;
    JsonValue * result = parse(tokens, &start);

    for (auto token : tokens) delete [] token.text;

    return result;
}

namespace {

void print_indent(JsonValue * value, size_t spaces) {
    char * indent = new char[spaces + 1];
    memset(indent, ' ', spaces);
    indent[spaces] = '\0';

    switch (value->type) {
    case JsonValueType::OBJECT: {
        JsonObject * object = static_cast<JsonObject *>(value);
        cout << indent << "[Object]" << endl;
        for (const auto & el : object->members) {
            cout << indent << el.first << ": " << endl;
            print_indent(el.second, spaces + 2);
        }
    } break;
    case JsonValueType::ARRAY: {
        JsonArray * array = static_cast<JsonArray *>(value);
        cout << indent << "[Array]" << endl;
        for (const auto & el : array->members) {
            print_indent(el, spaces + 2);
        }
    } break;
    case JsonValueType::NUMBER: {
        JsonNumber * number = static_cast<JsonNumber *>(value);
        printf("%s%f\n", indent, number->value);
    } break;
    case JsonValueType::STRING: {
        JsonString * string = static_cast<JsonString *>(value);
        printf("%s%s\n", indent, string->value.c_str());
    } break;
    default: break;
    }

    delete indent;
}

}

void print_json(JsonValue * value) {
    print_indent(value, 0);
}

}
