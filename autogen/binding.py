import os
import glob
from models import *
from log import autogen_logger
from utils import *
from parser import *
from jinja2 import Template, Environment, FileSystemLoader
import constants


def generate_bindings(settings: AutogenSettings) -> list[str]:
    """
    Handling all templates defined in settings.json blueprints section.

    :param settings: AutogenSettings dataclass instance, containing list of blueprints to process.
    :return: List of input blueprint file paths processed.
    """
    if settings.bindings is None:
        return []

    processed_files: list[str] = []

    for binding in settings.bindings:
        dep_files: list[str] = []

        for dep in binding.dependencies or []:
            final_dep = render_template_str(dep)
            files = glob.glob(os.path.join(BASE_DIR, final_dep))
            dep_files.extend([os.path.relpath(f, BASE_DIR) for f in files])

        dep_files = list(set(dep_files))

        for file in dep_files:
            is_cached = is_file_cached(file)
            autogen_logger.debug(f"Dependency cached status for {file}: {is_cached}")

            if is_cached:
                dep_files.remove(file)
                continue

        complete_input = os.path.join(BASE_DIR, binding.input)

        assert os.path.exists(
            complete_input
        ), f"Binding input file {binding.input} does not exist."

        is_input_cached = is_file_cached(binding.input)
        autogen_logger.debug(
            f"Input cached status for {binding.input}: {is_input_cached}"
        )
        if len(dep_files) > 0 or not is_input_cached:
            if not _generate_binding(binding):
                continue

        if not is_input_cached:
            processed_files.append(binding.input)

        processed_files.extend(dep_files)

    return list(set(processed_files))


def _generate_binding(binding: Binding) -> bool:
    """
    Using template for auto-create output file from blueprint file.

    :param binding: Binding dataclass instance, containing file and output paths. If the input file is cached and unchanged, skip regeneration.
    :return: True if needa regeneration, False otherwise.
    """
    copmlete_output = os.path.join(BASE_DIR, binding.output)
    complete_input = os.path.join(BASE_DIR, binding.input)

    assert os.path.exists(
        complete_input
    ), f"Binding input file {binding.input} does not exist."

    output_exist = os.path.exists(copmlete_output)

    if output_exist:
        return False

    parser = Parser()

    for include_path in binding.includes or []:
        final_path = render_template_str(
            include_path,
            INPUT=binding.input,
            OUTPUT=binding.output,
            TEMPLATE=binding.template,
        )
        parser.add_include_path(final_path)

    parser.parse(complete_input)

    env = Environment(
        loader=FileSystemLoader(os.path.join(BASE_DIR, "autogen", "templates"))
    )

    template: Template = env.get_template(binding.template)

    const_variables: dict[str, str] = {}

    for key, value in constants.__dict__.items():
        if key.isupper():
            const_variables[key] = value

    output_content = template.render(
        **const_variables,
        INPUT=binding.input,
        OUTPUT=binding.output,
        parser=parser,
    )

    os.makedirs(os.path.dirname(copmlete_output), exist_ok=True)

    with open(copmlete_output, "w") as f:
        f.write(output_content)

    autogen_logger.info(f"Generated binding: {binding.output} from {binding.input}")

    return True
