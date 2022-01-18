#include "Zlib.hpp"

#include <cassert>
#include <iostream>

namespace method {

namespace {

U8 max(U8 * array, U8 array_length) {
    U8 result = array[0];

    for (U8 i = 1; i < array_length; i++) {
        if (array[i] > result) result = array[i];
    }

    return result;
};

void print_code(U16 code, U8 length) {
    char result[17] = {0};
    for (int i = 16 - length; i < 16; i++) {
        result[15 - i] = (code & 1) ? '1' : '0';
        code >>= 1;
    }
    std::printf("%5s", result);
}

const U8 code_index[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

class BitStream {
private:
    U8 * byte_stream;
    U32 byte_stream_length;

    U32 cur_byte;
    U8 cur_bit;

public:
    // TODO: size_t where?
    BitStream(U8 * data, U32 length)
        : byte_stream(data),
          byte_stream_length(length),
          cur_byte(0),
          cur_bit(0) {}

    U8 peek(U8 bits) const {
        assert(bits <= 8);

        U8 result = 0;

        if (cur_bit + bits <= 8) {
            U8 mask = (U8)((U16)(1 << bits) - 1) << cur_bit;

            result = (byte_stream[cur_byte] & mask) >> cur_bit;
        } else {
            U8 mask1 = (U8)((U16)(1 << bits) - 1) << cur_bit;
            U8 mask2 = (U8)((U16)(1 << bits) - 1) >> (8 - cur_bit);

            U8 data1 = (byte_stream[cur_byte] & mask1) >> cur_bit;
            U8 data2 = (byte_stream[cur_byte + 1] & mask2) << (8 - cur_bit);

            result = data1 | data2;
        }

        return result;
    }

    void skip(U8 bits) {
        assert(bits <= 8);

        if (cur_bit + bits >= 8) {
            cur_byte++;
        }

        cur_bit = (cur_bit + bits) % 8;
    }

    U8 get(U8 bits) {
        assert(bits <= 8);

        U8 result = peek(bits);

        skip(bits);

        return result;
    }

    U8 get() {
        U8 result = byte_stream[cur_byte] & (1 << cur_bit) ? 1 : 0;

        if (cur_bit + 1 >= 8) {
            cur_byte++;
        }

        cur_bit = (cur_bit + 1) % 8;

        return result;
    }

    U8 * get_bytes(size_t bytes) {
        assert(cur_bit == 0);

        U8 * result = new U8[bytes];

        for (size_t i = 0; i < bytes; i++)
            result[i] = byte_stream[cur_byte++];

        return result;
    }
};

class HuffTree {
private:
    U8 max_depth;
    size_t max_index;
    I16 * value_tree;

public:
    HuffTree(U8 * lengths, U16 lengths_length)
        : max_depth(max(lengths, lengths_length) + 1),
          max_index(1ULL << max_depth),
          value_tree(new I16[max_index]()) {

        std::printf("HT size: 2^%u = %zu\n", max_depth, max_index);

        // This algorithm is completely lifted from RFC 1951
        U8 max_length = max(lengths, lengths_length);

        U8 * length_count = new U8[max_length]();
        for (U8 i = 0; i < lengths_length; i++) {
            if (lengths[i]) length_count[lengths[i]]++;
        }

        U16 * first_code = new U16[max_length]();
        U16 code = 0;
        for (U16 bits = 1; bits <= max_length; bits++) {
            code = (code + length_count[bits - 1]) << 1;
            first_code[bits] = code;
        }

        for (U16 i = 0; i <= lengths_length; i++) {
            if (lengths[i] != 0) {
                U16 value = code_index[i];
                U8 length = lengths[i];
                U16 code = first_code[length]++;

                print_code(code, length);

                U16 location = 0;
                for (U64 mask = 1ULL << (length - 1); mask > 0; mask >>= 1) {
                    if (code & mask) location = 2 * location + 2;
                    else location = 2 * location + 1;
                }

                // ~value = -(value + 1) by two's complement, cool trick right
                value_tree[location] = ~value;

                std::printf("(%d) -> %2d = tree[%d]\n", length, value, location);
            }
        }

        delete [] length_count;
        delete [] first_code;
    };

    ~HuffTree() {
        delete [] value_tree;
    }

    void print() const {
        for (size_t i = 0; i < max_index; i++) {
            if (value_tree[i]) std::printf("0x%zx -> %d, ", i, ~value_tree[i]);
        }
        std::printf("\n");
    }

    I16 parse_code(BitStream * bs) const {
        U16 location = 0;
        while (location < max_index && !value_tree[location]) {
            U8 bit = bs->get();
            //std::printf("%c", bit ? '1' : '0');
            if (bit) location = 2 * location + 2;
            else location = 2 * location + 1;
        }
        //std::printf("\n");

        if (location < max_index && value_tree[location]) {
            return ~value_tree[location];
        } else {
            //std::printf("failed to parse next bits, location was %d\n", location);
            return -1;
        }
    }
};

}

U8 * zlib_inflate(U8 * deflated_data, U32 deflated_size) {
    for (int i = 0; i < 100; i++) {
        char val = deflated_data[i];
        char result[9] = {0};
        for (int i = 1; i <= 8; i++) {
            result[8 - i] = (val & 1) ? '1' : '0';
            val >>= 1;
        }
        std::printf("%s\n", result);
    }

    U32 result_allocated = 1;
    U8 * result = new U8[result_allocated];

    BitStream in(deflated_data, deflated_size);

    U16 checksum = swap_endian(*(U16 *)(deflated_data));
    assert(checksum % 31 == 0);

    U8 comp_method = in.get(4);
    U8 comp_window = in.get(4);

    U8 LENGTHS[8] = {3, 3, 3, 3, 3, 2, 4, 4};
    HuffTree test_tree(LENGTHS, 8);

    in.skip(5);
    U8 flag_has_dict = in.get(1);
    U8 flag_compression = in.get(2);

    std::printf("comp_window: 2^%d\n", comp_window + 8);
    std::printf("comp_method: %d\n", comp_method);
    std::printf("flag_compression: %d\n", flag_compression);
    std::printf("flag_has_dict: %d\n", flag_has_dict);

    if (flag_has_dict) {
        U8 * dict = in.get_bytes(4);
        delete [] dict;
    }

    U8 block_last = 0;
    do {
        // TODO: just do one block
        block_last = in.get(1);
        U8 block_type = in.get(2);

        std::printf("block_last: %d\n", block_last);
        std::printf("block_type: %d\n", block_type);

        if (block_type == 0b00) {
            U16 block_length     = in.get(8) | (in.get(8) << 8);
            U16 not_block_length = in.get(8) | (in.get(8) << 8);

            assert(block_length == !not_block_length);

            std::printf("------------ NOT IMPLEMENTED --------------\n");

            // out_copy_bytes(block_length);
        } else if (block_type == 0b01) {

        } else if (block_type == 0b10) {
            U16 num_literals = 257 + in.get(5);
            U16 num_distances = 1 + in.get(5);
            U16 num_codes = 4 + in.get(4);

            std::printf("literals: %d\ndistances: %d\ncodes: %d\n", num_literals, num_distances, num_codes);

            U8 * code_length = new U8[num_codes]();
            for (U16 i = 0; i < num_codes; i++) {
                code_length[i] = in.get(3);
            }
            HuffTree code_code(code_length, num_codes);
            code_code.print();

            U8 * literal_length = new U8[num_literals]();
            U16 i = 0;
            while (i < num_literals) {
                I16 command = code_code.parse_code(&in);
                U16 last_value = literal_length[i - 1];
                switch (command) {
                case 16:
                    for (U16 extra = 0; extra < 3 + in.get(2); extra++)
                        literal_length[i++] = last_value;
                    break;

                case 17:
                    for (U16 extra = 0; extra < 3 + in.get(3); extra++)
                        literal_length[i++] = 0;
                    break;

                case 18:
                    for (U16 extra = 0; extra < 11 + in.get(7); extra++)
                        literal_length[i++] = 0;
                    break;

                default:
                    literal_length[i++] = command;
                    break;
                }
            }
            /*
            std::printf("literal lengths:\n");
            for (U16 i = 0; i < num_literals; i++)
                std::printf("%d ", literal_length[i]);
            std::printf("\n");
            */
            //HuffTree literal_code = HuffTree(literal_length, num_literals);
            code_code.print();

            U8 * distance_length = new U8[num_distances]();
            i = 0;
            while (i < num_distances) {
                I16 command = code_code.parse_code(&in);
                U16 last_value = distance_length[i - 1];
                switch (command) {
                case 16:
                    for (U16 extra = 0; extra < 3 + in.get(2); extra++)
                        distance_length[i++] = last_value;
                    break;

                case 17:
                    for (U16 extra = 0; extra < 3 + in.get(3); extra++)
                        distance_length[i++] = 0;
                    break;

                case 18:
                    for (U16 extra = 0; extra < 11 + in.get(7); extra++)
                        distance_length[i++] = 0;
                    break;

                default:
                    distance_length[i++] = command;
                    break;
                }
            }
            /*
            std::printf("distance lengths:\n");
            for (U16 i = 0; i < num_distances; i++)
                std::printf("%d ", distance_length[i]);
            std::printf("\n");
            */
            //HuffTree distance_code = HuffTree(distance_length, num_distances);
            code_code.print();

            // Here we would interpret the actual compressed data

            delete [] code_length;
            delete [] literal_length;
            delete [] distance_length;
        }
    } while (!block_last);

    return result;
}

}
