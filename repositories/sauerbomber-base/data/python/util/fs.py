import os
import os.path as path

def enshure_directory(file):
    """
    Enshure that a directory called 'file' extists.
    Throws an error if it is a file.
    """
    if not path.exists(file):
        os.makedirs(file)
    elif not path.isdir(file):
        raise EnvirementError(file.join([' is a file.']))
                              
def mk_empty_file(file):
    """
    Create an empty file at the specified location
    """
    o = open(file, 'w')
    o.write('')
    o.close()
                              
def enshure_file(file):
    """
    Enshure that a directory called 'file' extists.
    Throws an error if it is a dir.
    """
    if not path.exists(file):
        enshure_directory(path.dirname(file))
        mk_empty_file(file)
    elif not path.isfile(file):
        raise EnvirementError(file.join([' is a directory.']))

