#include "Context.hpp"
#include "View/game.hpp"
#include "uhcio.hh"

int main() {
    Context& ctx = Context::instance();
    ctx.init();

    {
        // Log::debug("setView");
        GameView gameView;
        // ctx.setVsync(VSYNC);
        // Log::debug("setView");
        ctx.setView(gameView);
        // Log::debug("run");
        ctx.run();
    }

    Log::debug("Main end");
    // logD("Main end");
}
