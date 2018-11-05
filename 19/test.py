import os
import pytest
import subprocess
import signal


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
    os.system('insmod eu19m.ko')
    DMESG.clean()
    yield "resource"
    os.system('rmmod eu19m')


class TestMyEu19(object):
    def test_full(self, module):
	port = 8080
	ID = "BitsCruncher"
	process = subprocess.Popen(
		"python -m SimpleHTTPServer {}".format(port), shell=True, preexec_fn=os.setsid)
	try:
		subprocess.call(
			['wget', 'localhost:{0}/{1}'.format(port, ID)])
		assert(DMESG.contains('Found {}'.format(ID)))
	finally:
		os.killpg(process.pid, signal.SIGTERM)
