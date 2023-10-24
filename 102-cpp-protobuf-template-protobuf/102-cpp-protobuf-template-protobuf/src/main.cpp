#include "message.pb.h"
#include "protobuf_parser/DelimitedMessagesStreamParser.hpp"
#include <iostream>


int main() {
    std::list<typename DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage>::PointerToConstValue> messages;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;
    TestTask::Messages::WrapperMessage message;
    std::string stream;
    for (int i = 0; i < 10; i++)
    {
        auto request = new TestTask::Messages::RequestForSlowResponse;
        request->set_time_in_seconds_to_sleep(2 * i);
        message.set_allocated_request_for_slow_response(request);
        auto data = serializeDelimited<TestTask::Messages::WrapperMessage>(message);
        for (int j = 0; j < data->size(); j++)
        {
            stream.push_back((*data)[j]);
        }
    }
    messages = parser.parse(stream);
    for (const auto& i : messages){
        std::cout << (*i).DebugString() << "\n";
    }


    return 0;
}
