#include "Context.hpp"
#include "View/menu.hpp"
#include "uhcio.hh"

int main() {
    Context& ctx = Context::instance();
    ctx.init();

    {
        Log::debug("menuView");
        App menuView;
        // ctx.setVsync(VSYNC);
        Log::debug("setView");
        ctx.setView(menuView);
        Log::debug("run");
        ctx.run();
    }

    Log::debug("Main end");
    // logD("Main end");
}
