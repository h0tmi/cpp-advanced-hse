#!/usr/bin/env python3
import argparse
import subprocess
import glob
import json
import os
import shutil

REPO_ROOT = os.path.dirname(os.path.realpath(__file__))
SHADOW_REPO_DIR = os.path.join(REPO_ROOT, '.submit-repo')
VERBOSE = False


def git(*args):
    if VERBOSE:
        print("> {}".format(" ".join(["git"] + list(args))))
        subprocess.check_call(" ".join(["git"] + list(args)), cwd=SHADOW_REPO_DIR, shell=True)
    else:
        subprocess.check_output(["git"] + list(args), cwd=SHADOW_REPO_DIR, stderr=subprocess.PIPE)


def git_output(*args):
    if VERBOSE:
        print("> {}".format(" ".join(["git"] + list(args))))

    return subprocess.check_output(["git"] + list(args)).strip().decode('utf-8')


def set_up_shadow_repo(task_name, user_name, user_email):
    # to fix access denied error occuring on Windows
    def onerror(func, path, exc_info):
        import stat
        if not os.access(path, os.W_OK):
            os.chmod(path, stat.S_IWUSR)
            func(path)
        else:
            raise

    try:
        shutil.rmtree(SHADOW_REPO_DIR, onerror=onerror)
    except FileNotFoundError:
        pass
    os.makedirs(SHADOW_REPO_DIR)

    git("init")
    git("remote", "add", "local", "..")
    try:
        git("fetch", "local", "+refs/heads/submits/{0}*:refs/heads/submits/{0}*".format(task_name))
    except:
        pass

    private_url = git_output("remote", "get-url", "student")
    git("remote", "add", "student", private_url)

    with open(os.path.join(SHADOW_REPO_DIR, ".git", "config"), mode="a", encoding="utf-8") as config:
        config.write("[user]\n\tname = {}\n\temail = {}\n".format(user_name, user_email))


def create_commits(task_name, files):
    git("checkout", "-b", "initial")
    shutil.copyfile(os.path.join(REPO_ROOT, ".grader-ci.yml"), os.path.join(SHADOW_REPO_DIR, ".gitlab-ci.yml"))
    git("add", ".gitlab-ci.yml")
    git("commit", "-m", "initial")

    try:
        git("checkout", "submits/" + task_name)
    except:
        git("checkout", "-b", "submits/" + task_name)

    try:
        os.makedirs(os.path.join(SHADOW_REPO_DIR, task_name))
    except:
        pass

    shutil.copyfile(os.path.join(REPO_ROOT, ".grader-ci.yml"), os.path.join(SHADOW_REPO_DIR, ".gitlab-ci.yml"))
    git("add", ".gitlab-ci.yml")

    for filename in files:
        directory = os.path.join(SHADOW_REPO_DIR, task_name, os.path.dirname(filename))
        for path in glob.glob(filename):
            try:
                os.makedirs(directory)
            except:
                pass
            shutil.copy(path, directory)

        git("add", task_name + "/" + filename)

    git("commit", "-m", task_name, "--allow-empty")


def push_branches(task_name):
    git("push", "local", "submits/" + task_name)
    try:
        git("push", "-f", "student", "initial")
    except:
        pass
    git(
        "push", "-f", "student", "submits/" + task_name,
        '-o', 'merge_request.create',
        '-o', 'merge_request.target=master',
        '-o', 'merge_request.title=' + task_name,
        '-o', 'merge_request.label=task/' + task_name
    )


def ensure_list(value):
    if not isinstance(value, list):
        return [value]
    return value


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-v", "--verbose",
        help="increase output verbosity",
        action="store_true"
    )

    parser.add_argument(
        "task_path",
        nargs='?',
        help="task relative path",
        default=''
    )

    args = parser.parse_args()
    VERBOSE = args.verbose

    task_name = args.task_path
    if task_name == '':
        real_current_path = os.path.realpath(".")
        task_group_name = os.path.basename(os.path.dirname(real_current_path))
        task_name = task_group_name + "/" + os.path.basename(real_current_path)

    task_config = None
    try:
        task_config = json.load(open(".tester.json"))
    except FileNotFoundError:
        print("error: Task config not found. Are you running tool from correct directory?")
        exit(1)

    user_name = git_output("config", "user.name")
    user_email = git_output("config", "user.email")

    set_up_shadow_repo(task_name, user_name, user_email)

    create_commits(task_name, ensure_list(task_config["allow_change"]))

    push_branches(task_name)
