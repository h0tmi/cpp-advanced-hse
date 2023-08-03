#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#undef CHECK
#undef INFO

#ifndef NDEBUG

#include <glog/logging.h>

class LoggerInitializer : public Catch::TestEventListenerBase {
public:
    using Catch::TestEventListenerBase::TestEventListenerBase;

    // NOLINTNEXTLINE
    void testRunStarting(Catch::TestRunInfo const &) override {
        // NOLINTNEXTLINE
        FLAGS_logtostderr = true;

        google::InitGoogleLogging("");

        // google::InstallFailureSignalHandler();

        DLOG(INFO) << "Hello from logging subsystem!";
    }
};

CATCH_REGISTER_LISTENER(LoggerInitializer);

#endif
