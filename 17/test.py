import pytest

eu17_id = "BitsCruncher"
device ="/dev/eudyptula"


def test_read():
    with open(device, 'r') as f:	
	assert(f.read() == eu17_id)

def test_read_in_parts():
    f = open(device, 'r')
    assert(eu17_id[:3] == f.read(3))
    assert(eu17_id[3:5] == f.read(2))
    assert(eu17_id[5:] == f.read())
    f.close()
 

def test_write_correct():
    with open(device, 'wb') as f:
        f.write(eu17_id)

def test_write_error():
    with pytest.raises(Exception):
        with open(device, 'wb') as f:
            f.write("not_id")
