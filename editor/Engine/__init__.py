import os
import sys

sys.path.append(os.path.dirname(os.path.abspath(__file__)))

try:
    from EngineCoreBinding import *  # type: ignore
except:
    from Engine.EngineCoreBinding import *  # type: ignore
