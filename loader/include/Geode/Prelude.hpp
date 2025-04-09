#pragma once

// Because C++ doesn't like using
// namespaces that don't exist
namespace freod {
    namespace addresser {}
    namespace cast {}
    namespace cocos {}
    namespace utils {}
    namespace helper {}
    namespace op {}
    namespace stream {}
    namespace view {}
}

namespace cocos2d {
    namespace extension {}
}

namespace freod {
    namespace prelude {
        using namespace ::freod;
        using namespace ::freod::addresser;
        using namespace ::freod::cast;
        using namespace ::freod::cocos;
        using namespace ::freod::helper;
        using namespace ::freod::utils;
        using namespace ::freod::op;
        using namespace ::freod::stream;
        using namespace ::freod::view;
        using namespace ::cocos2d;
        using namespace ::cocos2d::extension;
    }
}
