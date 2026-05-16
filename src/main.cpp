#include "Context.hpp"
// #include "Application.hpp"
// #include "Logger.hpp"
#include "uhcio.hh"

int main() {
    Context& ctx = Context::instance();
    ctx.init();

    // {
    //     App appView;
    //     // ctx.setVsync(VSYNC);
    //     ctx.setView(appView);
    //     ctx.run();
    // }

    Log::debug("Main end");
    // logD("Main end");
}
