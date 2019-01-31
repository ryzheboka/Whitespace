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
        return n

    @staticmethod
    def create_dict():
        """creates a dictionary of commands in Whitespace mapped to their equivalent in nasm"""

        d = dict()
        for file in os.listdir(os.path.dirname(os.path.abspath(__file__))+"/translations"):
            with open(os.path.dirname(os.path.abspath(__file__))+"/translations/" + file, "r") as f:
                d[os.path.splitext(file)[0]] = f.read()
        return d

    @staticmethod
    def translate(text):
        """Translates given text using the dictionary."""
        """p_ges is a regular expression which describes the Whitespace-language"""
        p_ges = re.compile(
            r'((AA[AB]*C)|(BCAA)|(BCAB)|(CCC)|'
            r'(ACC)|(ACA)|(ACB)|(BAAA)|(BAAB)|(BAAC)|(BABA)|(BABB)|(BBA)|(BBB)|(BCBA)|(BCBB)|'
            r'(CAA[AB]*C)|(CAC[AB]*C)|(CAB[AB]*C)|(CBA[AB]*C)|(CBC)|(CBB[AB]*C))')
        """p_with_n_as_arg represents all commands which take an Integer argument"""
        p_with_n_as_arg = re.compile('(AA)')
        """p_with_n_as_arg represents all commands which take a String as argument"""
        p_with_str_as_arg = re.compile('(CAA)|(CAC)|(CAB)|(CBA)|(CBB)')
        on2 = re.compile('(BBA)|(BBB)|(BCAB)|(BCBA)|(BCBB)|(CAB)')
        """counters are inserted into label names needed for function calls, so they (label names) are unique"""
        counter2 = 0
        counter3 = 0

        d = Translator.create_dict()    # dictionary with command translations

        res = ""
        while text:
            m = re.match(p_ges, text)
            if m:
                g = m.group()
                on2_arg = re.match(on2, g)
                n_arg = re.match(p_with_n_as_arg, g)
                str_arg = re.match(p_with_str_as_arg, g)
                if n_arg:
                    """the command takes an Integer argument"""
                    command = n_arg.group()
                    argument = g[n_arg.end():-1]
                    main_text = d[command]
                    argument = Translator.arg_to_n(argument)
                    """insert the argument inside the program"""
                    res += main_text.replace("argument1", str(argument))
                elif str_arg:
                    """the command takes a String-argument"""
                    command = str_arg.group()
                    argument = g[str_arg.end():-1]
                    argument = Translator.arg_to_n(argument)
                    main_text = d[command]
                    main_text = main_text.replace("argument1", "label_" + str(argument).replace("-", "0"))
                    if on2_arg:
                        main_text = main_text.replace("argument2", str(counter2))
                        counter2 += 1
                    """insert the argument inside the program"""
                    res += main_text
                else:
                    """the command takes no argument"""
                    main_text = d[g]
                    if on2_arg:
                        main_text = main_text.replace("argument2", str(counter2))
                        counter2 += 1
                    if g == "BCBB" or g == "BCBA":     # the command has two "technical" labels
                        main_text = main_text.replace("argument3", str(counter3))
                        counter3 += 1
                    res += main_text
                text = text[m.end():]
            else:
                raise Exception("Contains not implemented or not existing commands")
        return res


class Writer:
    def __init__(self, f_name, source_code):
        self.f = open(f_name, "w")  # .asm file
        self.text = source_code  # preprocessed sourcecode

    def write_head(self):
        """append the 'header' (same for each program translated into nasm (except the filename is inserted))"""

        with open(os.path.dirname(os.path.abspath(__file__))+"/start.txt", "r") as file:
            content = file.read().replace("filename", os.path.splitext(os.path.split(self.f.name)[1])[0])
            self.f.write(content)

    def write_body(self):
        """translate the source code, then write the translation into a file"""

        self.f.write(Translator.translate(self.text))

    def write_end(self):
        """finish the translation by adding the final part(same for each program translated
        into nasm, contains two functions for input and output and the .bss section)"""

        with open(os.path.dirname(os.path.abspath(__file__))+"/end.txt", "r") as file:
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
        p = re.compile(r'[^ \n\t\r\f]')
        text = p.sub("", text)
        text = text.replace(" ", "A")
        text = text.replace("\t", "B")
        text = text.replace("\n", "C")
        text = text.replace("\f", "C")

        return text.replace("\r", "C")
