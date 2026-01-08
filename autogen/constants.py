import os
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

try:
    from ..config.constants import *
except:
    from config.constants import *  # type: ignore
