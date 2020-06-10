#ifndef jukk_server_stateholder_h
#define jukk_server_stateholder_h

#include<string>
#include<map>
#include<ctime>

namespace Protocol
{
    class Stateholder
    {
        private:
            std::map<std::string, std::map<std::string,std::string>*> m_states;
            std::map<std::string, time_t> m_stateTimestamps;
            const int m_expiryTimeout_sec;

            int clearExpiredStates();
            int insertOrUpdateNowTimestamp(const std::string&);
            int deleteTimestamp(const std::string&);

        public:
            const std::map<std::string,std::string>* const getState(const std::string&);
            int insertOrUpdateState(std::string, std::map<std::string,std::string>&);
            int deleteState(const std::string&);
            Stateholder();
            ~Stateholder();

            void printStateInternals();
    };
};
#endif