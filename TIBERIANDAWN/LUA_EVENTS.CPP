#include "function.h"

void On_Scenario_Start(unsigned int scenarioNumber)
{
    if (!Lua_Is_Enabled())
    {
        return;
    }

    auto scriptBuffer = new char[1024];

    sprintf(
        scriptBuffer, "\
        for _, h in ipairs(Game.ScenarioStartEventHandlers) do   \n\
           local ok, err = pcall(h, %u)                          \n\
                                                                 \
            if not ok then                                       \n\
              showError(err)                                     \n\
            end                                                  \n\
         end\
        ",
        scenarioNumber
    );

    Execute_Lua_Script(scriptBuffer);

    delete scriptBuffer;
}
