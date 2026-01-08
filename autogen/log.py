import os
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
print(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

try:
    from ..config.utils.log import *
except:
    from config.utils.log import *  # type: ignore
