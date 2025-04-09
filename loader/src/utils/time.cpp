#include <Freod/utils/general.hpp>

#ifdef FREOD_IS_WINDOWS
    #define FREOD_CTIME() ctime_s(buf, sizeof buf, &t);
#else
    #define FREOD_CTIME() strcpy(buf, ctime(&t));
#endif

std::string freod::utils::timePointAsString(std::chrono::system_clock::time_point const& tp) {
    auto t = std::chrono::system_clock::to_time_t(tp);
    char buf[128];
    FREOD_CTIME();
    std::string res = buf;
    res.pop_back();
    return res;
}
