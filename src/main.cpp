#include "Context.hpp"
#include "View/menu.hpp"
#include "uhcio.hh"

int main() {
    Context& ctx = Context::instance();
    ctx.init();

    {
        MenuView gameView;
        ctx.viewPush(gameView);
        ctx.run();
    }

    Log::debug("Main end");
    // logD("Main end");
}
