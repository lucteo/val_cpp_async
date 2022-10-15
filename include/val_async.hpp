#pragma once

namespace val_async {
    struct scheduler {
        // TODO
    };

    template <typename T, typename F>
    struct Computation {
        F fun_;
        scheduler start_sched_;
    };
}