import os
import pytest
import subprocess
import timeit
import time
import subprocess

device ="/dev/eudyptula"

class DMESG(object):
	@staticmethod
	def contains(string):
		try:
			subprocess.check_output('dmesg | grep "{}"'.format(string), shell=True)
		except:
			return False
		return True
	
	@staticmethod
	def clean():
		subprocess.check_output("sudo dmesg -c", shell=True)

@pytest.fixture()
def module():
    os.system('insmod eu18.ko')
    DMESG.clean()
    yield "resource"
    os.system('rmmod eu18')

def is_eu_kthread_exist():
    ps_out = subprocess.check_output(['ps', 'aux'])
    return '[eudyptula]' in ps_out

def write_to_dev(string):
	with open(device, 'wb') as f:
            f.write(string)

class TestMyEu18(object):

    def test_write_by_all(self, module):
       st = os.stat(device)
       octal_triplet = oct(st.st_mode)[-3:]
       assert(octal_triplet == '222')

    def test_full(self, module):
        write_to_dev('omg')
	write_to_dev('omgomgomg')
	write_to_dev('12345678901234567890')
	time.sleep(21)
	assert(DMESG.contains('kthread got id 1 with name omg'))
	assert(DMESG.contains('kthread got id 2 with name omgomgomg'))
	assert(DMESG.contains('kthread got id 3 with name 1234567890123456789'))
	assert(DMESG.contains('kthread got id 4 with name 0'))
	write_to_dev('after1')
	write_to_dev('after2')
	time_remove = timeit.timeit("import os; os.system('rmmod eu18')", number=1)
	assert(5 > time_remove)
	os.system('insmod eu18.ko')
	
    def test_kthread(self, module):
        assert(True == is_eu_kthread_exist())
        
	time_remove = timeit.timeit("import os; os.system('rmmod eu18')", number=1)
	assert(5 > time_remove)
        assert(False == is_eu_kthread_exist())
        
	os.system('insmod eu18.ko')
 
