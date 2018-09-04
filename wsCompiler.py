import Helper
import sys
import os

if __name__ == "__main__":
    path = sys.argv[1]
    pre = Helper.Preproccessor(path)
    writer = Helper.Writer(os.path.splitext(path)[0] + ".asm", pre.remove_and_replace())
    writer.write_all()
