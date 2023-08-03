#!/usr/bin/env python3

import os
import shutil
import json

TASK_CONFIG_NAME = '.tester.json'
TASKS_FOLDER_NAME = './tasks'
PRIVATE_FOLDER_NAME = './private'


def patch():
    for (path, _, filenames) in os.walk(TASKS_FOLDER_NAME):
        if TASK_CONFIG_NAME in set(filenames):
            task_group_name = os.path.basename(os.path.dirname(path))
            task_name = os.path.basename(path)
            private_solution_path = os.path.join(PRIVATE_FOLDER_NAME, task_group_name, task_name)
            task_config = json.load(open(os.path.join(path, TASK_CONFIG_NAME)))
            if 'type' in task_config and task_config['type'] == 'ctf':
                print(f'Skipped CTF task "{task_group_name}/{task_name}" at path "{path}"')
                continue
            print(
                f'Found task "{task_group_name}/{task_name}" at path "{path}", patching from "{private_solution_path}"')
            shutil.copytree(private_solution_path, path, dirs_exist_ok=True)


if __name__ == '__main__':
    patch()
