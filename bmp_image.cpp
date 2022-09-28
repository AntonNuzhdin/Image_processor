#include "bmp_image.h"

#include <fstream>
#include <iostream>

namespace BmpFormat {
BMPHeader ReadHeader(std::ifstream& in) {
    BMPHeader header;
    ReadNum(in, header.file_type);
    ReadNum(in, header.file_size);
    ReadNum(in, header.reserved1);
    ReadNum(in, header.reserved2);
    ReadNum(in, header.offset_data);
    return header;
}

DIBHeader ReadDibHeader(std::ifstream& in) {
    DIBHeader header;
    ReadNum(in, header.header_size);
    ReadNum(in, header.size.width);
    ReadNum(in, header.size.height);
    ReadNum(in, header.color_planes);
    ReadNum(in, header.bits_per_pixel);
    ReadNum(in, header.compression_method);
    ReadNum(in, header.bitmap_data_size);
    ReadNum(in, header.resolution.horizontal);
    ReadNum(in, header.resolution.vertical);
    ReadNum(in, header.colors.total);
    ReadNum(in, header.colors.important);
    return header;
}

void WriteDibHeader(std::ofstream& out, const DIBHeader& header) {
    WriteNum(out, header.header_size);
    WriteNum(out, header.size.width);
    WriteNum(out, header.size.height);
    WriteNum(out, header.color_planes);
    WriteNum(out, header.bits_per_pixel);
    WriteNum(out, header.compression_method);
    WriteNum(out, header.bitmap_data_size);
    WriteNum(out, header.resolution.horizontal);
    WriteNum(out, header.resolution.vertical);
    WriteNum(out, header.colors.total);
    WriteNum(out, header.colors.important);
}

void WriteHeader(std::ofstream& out, const BMPHeader& header) {
    WriteNum(out, header.file_type);
    WriteNum(out, header.file_size);
    WriteNum(out, header.reserved1);
    WriteNum(out, header.reserved2);
    WriteNum(out, header.offset_data);
}

Image Load(std::string_view file_name) {
    std::ifstream in{file_name, std::ios::binary};
    BMPHeader bmp_header = ReadHeader(in);
    DIBHeader dib_header = ReadDibHeader(in);
    if (dib_header.header_size != 40) {
        throw MyExceptionBmp("Error! Incorrect DIB header size.");
    }
    if (bmp_header.file_type != BMP_FORMAT) {
        throw MyExceptionBmp("Error! Unrecognized file format.");
    }
    Image image{dib_header.size.width, dib_header.size.height};
    in.seekg(bmp_header.offset_data, std::ios_base::beg);

    const size_t padding = (4 - (dib_header.size.width * dib_header.bits_per_pixel) % 4) % 4;
    for (auto i = 0; i < dib_header.size.height; ++i) {
        for (auto j = 0; j < dib_header.size.width; ++j) {
            Color c;
            ReadNum(in, c.r);
            ReadNum(in, c.g);
            ReadNum(in, c.b);
            image.GetBitmapPix(dib_header.size.height - i - 1, j) = c;
        }
        in.ignore(padding);
    }
    return image;
}

void WriteFile(const Image& bitmap, const std::string_view path) {
    BMPHeader bmp_header;
    bmp_header.file_type = BMP_FORMAT;
    bmp_header.file_size = sizeof(BMPHeader) + sizeof(DIBHeader) + bitmap.GetHeight() * bitmap.GetWidth() * 3;
    bmp_header.offset_data = sizeof(BMPHeader) + sizeof(DIBHeader);
    DIBHeader dib_header;

    dib_header.size.width = bitmap.GetWidth();
    dib_header.size.height = bitmap.GetHeight();
    auto row_length = bitmap.GetWidth() * dib_header.bits_per_pixel;
    auto padding = ((row_length + 3) / 4) * 4 - row_length;
    dib_header.bitmap_data_size = bitmap.GetHeight() * (row_length + padding);

    std::ofstream out{path, std::ofstream::binary};
    WriteHeader(out, bmp_header);
    WriteDibHeader(out, dib_header);
    for (int i = static_cast<size_t>(bitmap.GetHeight() - 1); i >= 0; --i) {
        for (size_t j = 0; j < bitmap.GetWidth(); ++j) {
            WriteNum(out, bitmap.GetBitmapPix(i, j).r);
            WriteNum(out, bitmap.GetBitmapPix(i, j).g);
            WriteNum(out, bitmap.GetBitmapPix(i, j).b);
        }
        static const uint32_t rubbish = 0x55AA55AA;
        out.write(reinterpret_cast<const char*>(&rubbish), padding);
    }
    out.close();
}
}  // namespace BmpFormat
