
#include "debug.h"

/* IES assert handler: always breaks. */
SDL_AssertState sota_assert_handler(const SDL_AssertData *data, void *userdata) {
    return (SDL_ASSERTION_BREAK);
}
