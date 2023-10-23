/*
 * helpers.h
 *
 *  Created on: 2 Feb 2023
 *      Author: sia
 */
#include <boost/make_shared.hpp>
#include <google/protobuf/io/coded_stream.h>
#include <vector>

#ifndef SRC_PROTOBUF_PARSER_HELPERS_H_
#define SRC_PROTOBUF_PARSER_HELPERS_H_

#if GOOGLE_PROTOBUF_VERSION >= 3012004
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSizeLong())
#else
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSize())
#endif
typedef std::vector<char> Data;
typedef std::shared_ptr<const Data> PointerToConstData;

template <typename Message> PointerToConstData serializeDelimited(const Message& msg) {
    uint32_t messageSize = PROTOBUF_MESSAGE_BYTE_SIZE(msg);
    size_t headerSize = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);

    Data data(headerSize + messageSize);
    google::protobuf::uint8* buffer = reinterpret_cast<google::protobuf::uint8*>(&*data.begin());
    google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(messageSize, buffer);
    msg.SerializeWithCachedSizesToArray(buffer + headerSize);
    return std::make_shared<Data>(data);

}

/*!
 * \brief Расшифровывает сообщение, предваренное длиной из массива байтов.
 *
 * \tparam Message Тип сообщения, для работы с которым предназначена данная
 * функция.
 *
 * \param data Указатель на буфер данных.
 * \param size Размер буфера данных.
 * \param bytesConsumed Количество байт, которое потребовалось для расшифровки
 * сообщения в случае успеха.
 *
 * \return Умный указатель на сообщение. Если удалось расшифровать сообщение, то
 * он не пустой.
 */
template <typename Message>
std::shared_ptr<Message> parseDelimited(const void *data, size_t size,
                                        size_t* bytesConsumed = 0)
{
    try {
        if (data == nullptr) {
            //throw std::runtime_error("");
        }
        if (data && size != 0) {
            std::string s = reinterpret_cast<const char *>(static_cast<const Data *>(data));
            int sz = s.size();
            if (sz != size) {
                size=sz;
            }
            //const Data* ddata= static_cast<const Data *>(data);


            Data ddd;

            auto gaf=data;
            auto ConstData = static_cast<const Data *>(data);
            if (ConstData==NULL){
                throw 1;
            }

            if (ConstData->size() != size) {
                size = ConstData->size();
            }
            Data data1(ConstData->begin(), ConstData->end());
            //Data data1(s.begin(), s.end());


            uint32_t messageSize = 0;
            size_t headerSize = 0;
            std::shared_ptr<Message> res = std::make_shared<Message>(Message());
            google::protobuf::uint8 *buffer = reinterpret_cast<google::protobuf::uint8 *>(&*(data1.begin()));
            google::protobuf::io::CodedInputStream stream(buffer, size);
            std::string str;

            if (stream.ReadVarint32(&messageSize)) {
                headerSize = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);
            } else {
                messageSize = 0;
            }

            if (stream.ReadString(&str, messageSize)) {
                if (res->ParseFromString(str)) {
                    if (bytesConsumed) {
                        *bytesConsumed = headerSize + messageSize;
                    }

                    return res;
                } else
                    throw std::runtime_error("");


            }


        }

        if (bytesConsumed) {
            *bytesConsumed = 0;
        }
    }catch (const std::string& ex){
        throw std::runtime_error(ex);
    }
    catch (...){
        throw std::runtime_error("");
    }
    return nullptr;
}






#endif /* SRC_PROTOBUF_PARSER_HELPERS_H_ */
