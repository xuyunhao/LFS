#!/usr/bin/env python2.7
import sys
import os
import shutil
import subprocess
import optparse
import unittest
import stat

class TestBase(unittest.TestCase):

    def setUp(self):
        print "setup"

    def _nuke(self):
        global options
        print "nuke"
        subprocess.call(['umount', options.mount])

    def tearDown(self):
        print "tearDown"
        self._nuke()

    def _mklfs(bs = 1024, ss = 32, ns = 100):
        subprocess.call(['mklfs'])

    def _lfs(interval = 10, cache = 10):
        global options
        subprocess.call(['lfs', options.flash, options.mount])

class TestBasic(TestBase):

    def test_00_write_then_read(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        fd = open(path, "r")
        contents = fd.read()
        fd.close()
        self.assertEqual(contents, "hello")

    def test_01_write_and_write_and_read(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.write("goodbye")
        fd.close()
        fd = open(path, "r")
        contents = fd.read()
        fd.close()
        self.assertEqual(contents, "hellogoodbye")

    def test_02_seek(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        fd = open(path, "r")
        fd.seek(3)
        contents = fd.read()
        fd.close()
        self.assertEqual(contents, "lo")

    def test_03_read_from_hole(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.seek(10)
        fd.write("hello")
        fd.close()
        fd = open(path, "r")
        fd.seek(3)
        contents = fd.read(3)
        fd.close()
        self.assertEqual(contents, "\x00\x00\x00")

    def test_04_read_beyond_eof(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        fd = open(path, "r")
        fd.seek(10)
        contents = fd.read()
        fd.close()
        self.assertEqual(contents, "")

    def test_05_overwrite(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        fd = open(path, "w")
        fd.write("HELLO")
        fd.close()
        fd = open(path, "r")
        contents = fd.read()
        fd.close()
        self.assertEqual(contents, "HELLO")

    def test_06_read_across_eof(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        fd = open(path, "r")
        contents = fd.read(10)
        fd.close()
        self.assertEqual(contents, "hello")

    def test_07_read_across_eof(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        fd = open(path, "r")
        contents = fd.read(10)
        fd.close()
        self.assertEqual(contents, "hello")


    def test_08_large_file(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        expected = "a" * 3 * 1024
        fd.write(expected)
        fd.close()
        fd = open(path, "r")
        contents = fd.read()
        fd.close()
        self.assertEqual(contents, expected)

    def test_09_binary(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("\xaa\xbb\xcc")
        fd.close()
        fd = open(path, "r")
        contents = fd.read(10)
        fd.close()
        self.assertEqual(contents, "\xaa\xbb\xcc")

    def test_10_not_found(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        path = os.path.join(options.mount, "bar")
        self.assertRaises(Exception, open, path, "r")

    def test_10_stat_type(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        s = os.stat(path)
        self.assertTrue(stat.S_ISREG(s.st_mode))


    def test_11_stat_size(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        s = os.stat(path)
        self.assertEqual(s.st_size, 5)


    def test_12_stat_times(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        s = os.stat(path)
        self.assertTrue(s.st_atime == s.st_mtime and s.st_atime == s.st_ctime)

    def test_13_truncate_to_zero(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        fd = open(path, "a")
        fd.truncate(0)
        fd.close()
        fd = open(path, "r")
        contents = fd.read()
        fd.close()
        self.assertEqual(contents, "")

    def test_14_truncate_smaller(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        fd = open(path, "a")
        fd.truncate(3)
        fd.close()
        fd = open(path, "r")
        contents = fd.read()
        fd.close()
        self.assertEqual(contents, "hel")

    def test_15_truncate_larger(self):
        global options
        path = os.path.join(options.mount, "foo")
        fd = open(path, "w")
        fd.write("hello")
        fd.close()
        fd = open(path, "a")
        fd.truncate(10)
        fd.close()
        fd = open(path, "r")
        contents = fd.read()
        fd.close()
        self.assertEqual(contents, "hello\x00\x00\x00\x00\x00")

    def test_16_multiple_files(self):
        global options
        path1 = os.path.join(options.mount, "foo")
        fd = open(path1, "w")
        fd.write("hello")
        fd.close()
        path2 = os.path.join(options.mount, "bar")
        fd = open(path2, "w")
        fd.write("goodbye")
        fd.close()
        fd = open(path1, "r")
        contents1 = fd.read()
        fd.close()
        fd = open(path2, "r")
        contents2 = fd.read()
        fd.close()
        self.assertTrue(contents1 == "hello" and contents2 == "goodbye")


def main(args):
    global options

    usage = "Usage %prog [options] release"
    parser = optparse.OptionParser(version="1.0", usage=usage)

    parser.add_option("-q", "--quiet",
                      action="store_const", const=0, dest="verbosity", default=1,
                      help="quiet output")

    parser.add_option("-v", "--verbose",
                      action="count", dest="verbosity",
                      help="increase verbosity")


    parser.add_option("-F", "--failfast",
                      action="store_true", default=False, dest="failfast",
                      help="stop on first failed test")

    parser.add_option("-f", "--flash",
                      action="store", default=None, dest="flash",
                      help="flash device")

    parser.add_option("-m", "--mount",
                      action="store", default=None, dest="mount",
                      help="mount point")

    (options, args) = parser.parse_args(args)

    if options.flash is None:
        raise Exception("You must specify a flash device")

    if options.mount is None:
        raise Exception("You must specify a mount point")

    unittest.main(argv=args, verbosity=options.verbosity, failfast=options.failfast)

if __name__ == '__main__': 
    main(sys.argv)
