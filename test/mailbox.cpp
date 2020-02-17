/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * pvxs is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <iostream>

#include <pvxs/sharedpv.h>
#include <pvxs/server.h>
#include <pvxs/nt.h>
#include <pvxs/log.h>

DEFINE_LOGGER(app, "mailbox");

namespace {
using namespace pvxs;
using namespace pvxs::server;

void usage(const char* cmd)
{
    std::cerr<<"Usage: "<<cmd<<" <pvname>\n";
}

} // namespace

int main(int argc, char* argv[])
{
    if(argc<=1) {
        usage(argv[0]);
        return 1;
    }

    pvxs::logger_level_set(app.name, pvxs::Level::Info);
    pvxs::logger_config_env();

    auto initial = nt::NTScalar{TypeCode::Float64}.create();
    initial["value"] = 42.0;
    initial["alarm.severity"] = 0;
    initial["alarm.status"] = 0;
    initial["alarm.message"] = "";

    auto pv(SharedPV::buildMailbox());
    pv.open(initial);

    auto serv = Config::from_env()
            .build()
            .addPV(argv[1], pv);

    std::cout<<"Effective config\n"<<serv.config();

    log_printf(app, Info, "Running\n%s", "");
    serv.run();

    return 0;
}