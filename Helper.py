import re
import os


class Translator:

    @staticmethod
    def arg_to_n(arg):
        """Translates a number from the whitespace notation into an integer.
            In Whitespace, numbers are represented as a series of A and B
            terminated by a	C. A represents the binary digit 0, B represents 1. The sign
            of a number is given by its first character, A for positive and B for negative.
            Note that this is not twos complement, it just indicates a sign."""

        arg = arg.replace("A", "0")
        arg = arg.replace("B", "1")
        n = int(arg[1:], 2)
        if arg[0] == "1":
            n = - n
        return str(n)

    @staticmethod
    def create_dict():
        """creates a dictionary of commands in Whitespace mapped to their equivalent in nasm"""

        d = dict()
        for file in os.listdir("translations"):
            with open("translations/" + file, "r") as f:
                d[os.path.splitext(file)[0]] = f.read()
        return d

    @staticmethod
    def translate(text):
        """Translates given text using the dictionary."""

        """p_ges is a regular expression which describes the implemented part of the Whitespace-language"""
        p_ges = re.compile(r'((AA[AB]*C)|(BCAA)|(BCAB)|(CCC)|(ACC)|(ACA)|(ACB)|(BAAA)|(BAAB)|(BAAC)|(BABA)|(BABB))')
        """p_with_args represents all implemented commands which take arguments so they need to be handled separately"""
        p_with_args = re.compile('AA')

        d = Translator.create_dict()
        res = ""
        while text:
            m = re.match(p_ges, text)
            if m:
                g = m.group()
                pref = re.match(p_with_args, g)
                if pref:
                    """the command takes an argument"""
                    command = pref.group()
                    argument = g[pref.end():-1]
                    main_text = d[command]
                    argument = Translator.arg_to_n(argument)
                    """insert the argument inside the program"""
                    res += main_text.replace("argument", argument)
                else:
                    """the command takes no argument"""
                    res += d[g]
                text = text[m.end():]
            else:
                raise Exception("Contains not implemented or not existing commands")

        return res


class Writer:

    def __init__(self, f_name, source_code):
        self.f = open(f_name, "w")   # .asm file
        print(source_code)
        self.text = source_code    # preprocessed sourcecode

    def write_head(self):
        """append the 'header' (same for each program translated into nasm (except the filename is inserted))"""

        with open("start.txt", "r") as file:
            content = file.read().replace("filename", os.path.splitext(os.path.split(self.f.name)[1])[0])
            self.f.write(content)

    def write_body(self):
        """translate the source code, then write the translation into a file"""

        self.f.write(Translator.translate(self.text))


    def write_end(self):
        """finish the translation by adding the final part(same for each program translated
        into nasm, contains two functions for input and output and the .bss section)"""

        with open("end.txt", "r") as file:
            self.f.write(file.read())
        self.f.close()

    def write_all(self):
        self.write_head()
        self.write_body()
        self.write_end()


class Preproccessor:

    def __init__(self, path):
        self.f = open(path, "r")

    def remove_and_replace(self):
        """removes non-whitespace characters and replaces all spaces by A, tabs by B and newlines by C
           so the compiler becomes more readable"""

        text = self.f.read()
        p = re.compile(r'[^ \n\t]')
        text = p.sub("", text)
        text = text.replace(" ", "A")
        text = text.replace("\t", "B")
        return text.replace("\n", "C")
