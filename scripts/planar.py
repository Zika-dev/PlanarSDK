from os import environ, getcwd, path
import sys
import shutil
from pathlib import Path


# Color macros
class Color:
    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    RESET = '\033[0m'


def red(text):
    return f'{Color.RED}{text}{Color.RESET}'


def green(text):
    return f'{Color.GREEN}{text}{Color.RESET}'


def yellow(text):
    return f'{Color.YELLOW}{text}{Color.RESET}'


planar_path = environ.get('PLANAR_SDK')

if planar_path is None:
    print(red('Planar SDK installation not found, make sure to install it before running this script!'))
    sys.exit(1)

print(f'Planar SDK: {planar_path}')

planar_path = Path(planar_path)

cur_dir = Path(getcwd())

# Renames the file name and its content
def rename_template_file(file_path, proj_name, rename_content=True, additional_name=""):
    path = Path(file_path)

    proj_name = proj_name + additional_name

    if not path.is_file():
        print(red(f"Error: The file '{file_path}' does not exist!"))
        sys.exit(1)

    print(f'Processing file: {path.name}')

    # Read and replace the original content
    if rename_content:
        content = path.read_text()
        updated_content = content.replace('TemplateGame' + additional_name, proj_name)
        path.write_text(updated_content)

    # Rename the actual file
    full_suffix = "".join(path.suffixes)
    new_path = path.with_name(f"{proj_name}{full_suffix}")
    path.rename(new_path)
    print(green(f'Successfully renamed to: {new_path.name}'))


def create_project(name):
    print(f'Scaffolding {name}...')

    proj_dir = Path(cur_dir / f'{name}')

    if path.isdir(proj_dir):
        print(yellow(f'Project {name} already exists in folder! Delete it to create a new one'))
        sys.exit(1)

    print('Copying template...')

    shutil.copytree(planar_path / 'TemplateGame', proj_dir)

    print(green(f'Copied template to {proj_dir}'))

    # Rename project files
    print('Renaming project files...')

    rename_template_file(proj_dir / "TemplateGame.slnx", name)
    rename_template_file(proj_dir / "TemplateGame.vcxproj", name)
    rename_template_file(proj_dir / "TemplateGame.vcxproj.filters", name)
    rename_template_file(proj_dir / "TemplateGame.vcxproj.user", name, False)

    # cpp/hpp
    rename_template_file(proj_dir / "src/TemplateGameEntry.cpp", name, True, "Entry")
    rename_template_file(proj_dir / "src/TemplateGameEntry.hpp", name, False, "Entry")

    print(green(f'\n{name} scaffolded successfully!'))


proj_name = ""
if len(sys.argv) != 2:
    print(red(f"Creating a new project requires 1 argument (project name), got {len(sys.argv) - 1}!"))
    sys.exit(1)

proj_name = sys.argv[1]

create_project(proj_name)
print(Color.RESET)