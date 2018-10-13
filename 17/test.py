import os
import pytest
import subprocess

eu17_id = "BitsCruncher"
device ="/dev/eudyptula"

def test_write_by_all():
    st = os.stat(device)
    octal_triplet = oct(st.st_mode)[-3:]
    assert(octal_triplet == '222')

def test_write_correct():
    with open(device, 'wb') as f:
        f.write(eu17_id)

def test_write_error():
    with pytest.raises(Exception):
        with open(device, 'wb') as f:
            f.write("not_id")

def is_eu_kthread_exist():
    ps_out = subprocess.check_output(['ps', 'aux'])
    return '[eudyptula]' in ps_out


def test_kthread():
    assert(True == is_eu_kthread_exist())
    os.system('rmmod eu17')
    assert(False == is_eu_kthread_exist())
    os.system('insmod eu17.ko')
 
