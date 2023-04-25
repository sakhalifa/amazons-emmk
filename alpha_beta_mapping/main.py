import os
import subprocess

MAX_VALUE = 100

if __name__ == "__main__":

    successes = []
    for i in range(MAX_VALUE):
        
        agressivity = i
        defense = MAX_VALUE - agressivity
        os.system(f"AGG_COEF={agressivity} DEF_COEF={defense} make -j -B alphabeta_make")
        result = subprocess.check_output("./install/server -s 123456789 -m 10 ./install/alphabeta1.so ./alphabeta_make.so", shell=True).decode()

        if "Player 1" in result.split("\n")[-2]:
            successes.append(f"({agressivity}, {defense})")


    print("\n".join(successes))
