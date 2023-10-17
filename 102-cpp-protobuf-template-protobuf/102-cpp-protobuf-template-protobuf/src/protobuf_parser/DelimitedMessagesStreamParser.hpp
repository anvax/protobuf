/*
 * DelimitedMessagesStreamParser.hpp
 *
 *  Created on: 2 Feb 2023
 *      Author: sia
 */

#ifndef SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_HPP_
#define SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_HPP_
#include "helpers.hpp"
#include <list>

template <typename MessageType>
class DelimitedMessagesStreamParser
{
public:
    typedef std::shared_ptr<const MessageType> PointerToConstValue;
    typedef std::list<PointerToConstValue> ParsedMsgsList;
    std::list<PointerToConstValue> parse(const std::string& data);

private:
    std::vector<char> m_buffer;

    void addToBuffer(const std::string &data){
        m_buffer.insert(m_buffer.cend(), data.cbegin(), data.cend());
    };
    void deleteFromBuffer(size_t size){
        m_buffer.erase(m_buffer.cbegin(), m_buffer.cbegin() + size);
    };

};
template<typename MessageType>
typename DelimitedMessagesStreamParser<MessageType>::ParsedMsgsList DelimitedMessagesStreamParser<MessageType>::parse(
        const std::string &data) {
    addToBuffer(data);

    std::list<PointerToConstValue> list;
    size_t usedBytes = -1;

    while (usedBytes != 0)
    {
        auto parsedMsg = parseDelimited<MessageType>(static_cast<const void*>(&m_buffer),m_buffer.size(),&usedBytes);
        if (parsedMsg != nullptr){
            list.push_back(parsedMsg);
            deleteFromBuffer(usedBytes);
        }
    }

    return list;
}


#endif /* SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_HPP_ */
