#include <iostream>
#include <map>
class MemDump {
    static map <size_t, string> prevMap;
    static map <size_t, string> curMap;
public:
    static void createDump(+map <size_t, string> inputMap) {
        prevMap = curMap;
        curMap = inputMap;
        cout << "MEMORY\n       00     01     02     03     04     05     06     07     08     09" << endl;
        string line;
        for (int i = 0; i < 100; i++) {
            if (i % 10 == 0) {
                if (i == 0)
                    line = "00";

                else
                    line = to_string(i);
            }
            string memory = inputMap[i];
            int addZero = 5 - memory.length();

            for (int j = 0; j < addZero; j++)
                memory.insert(1, "0");

            line += "  " + memory;
            if (i % 10 == 9)
                cout << line << endl;
        }
        for (int i = 0; i < 19; i++){
            cout << endl;
        }
    }
private:
    MemDump(){};
};