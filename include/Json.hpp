#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "File.hpp"

namespace method {

enum JsonValueType {
    OBJECT,
    ARRAY,
    NUMBER,
    STRING
};

class JsonValue {
public:
    const JsonValueType type;

    JsonValue(JsonValueType type);
};

class JsonObject : public JsonValue {
public:
    std::unordered_map<std::string, JsonValue *> members;

    JsonObject();

    void add(std::string key, JsonValue * value);
    bool has(std::string key) const;

    JsonValue * get(std::string key) const;
    JsonValue * operator [] (std::string key) const;
};

class JsonArray : public JsonValue {
public:
    std::vector<JsonValue *> members;

    JsonArray();

    void add(JsonValue * value);
    bool has(size_t key) const;

    size_t length() const;

    JsonValue * get(size_t key) const;
    JsonValue * operator [] (size_t key) const;
};

class JsonNumber : public JsonValue {
public:
    const double value;

    JsonNumber(double value);
};

class JsonString : public JsonValue {
public:
    const std::string value;

    JsonString(std::string value);
};

JsonValue * load_json(const File & file);

void print_json(JsonValue * value);

}
