import os
import json
from dacite import from_dict
from utils import *
from dataclasses import asdict
from common_models.application_context import ApplicationSetting, ApplicationContext
from di import *
from .main_window_model import EditorMainWindowModel
from constants import *


@as_singleton
@as_dependencies(EditorMainWindowModel)
class EditorMainWindowViewModel:
    def __init__(self, model: EditorMainWindowModel) -> None:
        self.model = model

    def load_appsetting(self) -> None:
        app_context = di_get(ApplicationContext)
        assert app_context.app_settings is None and ApplicationSetting is not None
        app_context.app_settings = ApplicationSetting()

        editor_logger.info("Loading application settings...")

        if not os.path.exists(APP_SETTINGS_FILE):
            with open(APP_SETTINGS_FILE, "w", encoding="utf-8") as f:
                json.dump(asdict(app_context.app_settings), f, indent=4)

        with open(APP_SETTINGS_FILE, "r", encoding="utf-8") as f:
            data = json.load(f)
            app_context.app_settings = from_dict(
                data_class=type(app_context.app_settings),
                data=data,
            )

    def load_default_project(self) -> None:
        app_context = di_get(ApplicationContext)
        assert app_context.app_settings is not None

        if len(app_context.app_settings.recentProjects) == 0:
            return

        default_project_path = app_context.app_settings.recentProjects[0]
        if not os.path.exists(default_project_path):
            app_context.app_settings.recentProjects.pop(0)
            app_context.save_app_settings()
            return

        editor_logger.info(f"Loading default project from path: {default_project_path}")
        app_context.load_project_from_path(default_project_path)
