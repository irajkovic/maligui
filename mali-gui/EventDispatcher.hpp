#pragma once

#include <functional>
#include <vector>

namespace maligui
{

template <typename... PARAMS>
class EventDispatcher
{
        using Slot = std::function<void(PARAMS...)>;

        template <typename T>
        Slot makeFunction(T* object, void(T::*func)(PARAMS...)) {
                return [object, func](PARAMS... params) -> void {
                        (object->*(func))(params...);
                };
        }

public:
        template <typename T>
        void attach(T* object, void(T::*func)(PARAMS...)) {
               attach(makeFunction(object, func));
        }

        void attach(Slot s) {
                mSlots.push_back(s);
        }

        void dispatch(PARAMS... params) {
                for (auto& slot : mSlots) {
                        slot(params...);
                }
        }

private:
        std::vector<Slot> mSlots;

};

} // namespace maligui
