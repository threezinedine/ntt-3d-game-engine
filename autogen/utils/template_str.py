from jinja2 import Template
from typing import Any
import constants


def render_template_str(template_str: str, **kwargs: Any) -> str:
    """
    Render a template string using Jinja2 with the provided variables.

    :param template_str: The template string to be rendered.
    :param variables: A dictionary of variables to be used in the template.
    :return: The rendered string.
    """
    template = Template(template_str)

    const_variables: dict[str, str] = {}

    for key, value in constants.__dict__.items():
        if key.isupper():
            const_variables[key] = value
    output_content = template.render(
        **const_variables,
        **kwargs,
    )

    return output_content
