#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import time
import threading
import Queue
import argparse
import logging
import serial

#
# Data logger
# to be used in conjunction with the MAX30100_RawData example
#


logger = logging.getLogger(__name__)


class SerialStreamer(threading.Thread):
    DEFAULT_SPEED = 115200
    DEFAULT_HOLDOFF = 1.5
    DEFAULT_TIMEOUT = .1

    def __init__(self, port, holdoff=DEFAULT_HOLDOFF, speed=DEFAULT_SPEED):
        super(SerialStreamer, self).__init__()
        self._s = serial.Serial(port, speed, timeout=.5)
        self._q = Queue.Queue()
        self._holdoff = holdoff
        self._time_started = 0

    def run(self):
        logger.info('Starting streamer thread')
        self._isRunning = True

        time.sleep(self._holdoff)
        self._s.flushInput()

        self._time_started = time.time()

        while self._isRunning:
            self._poll()

    def stop(self):
        logger.info('Stopping streamer thread')
        self._isRunning = False
        self.join()

    def get_samples(self):
        samples = []
        while not self._q.empty():
            samples.append(self._q.get())

        return samples

    def _poll(self):
        line = self._s.readline().strip()
        if line:
            try:
                a, b = [int(v) for v in line.split('\t')]
            except Exception, e:
                logger.exception(e)

            self._q.put((self._get_time_offs(), a, b))

    def _get_time_offs(self):
        return int((time.time() - self._time_started) * 1000.0)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('port', help='Serial port of the device')
    parser.add_argument('outfile', help='Output recording file')
    parser.add_argument('--holdoff', type=float, default=1.5,
                        help='Settlement time after connection')
    parser.add_argument('--debug', action='store_true', help='Be verbose')

    return parser.parse_args()

def gather(streamer, fp):
    samples_count = 0

    while True:
        samples = streamer.get_samples()
        samples_count += len(samples)

        for sample in samples:
            fp.write('%d\t%d\t%d\n' % sample)
        logger.info('Gathered %d samples' % samples_count)

        time.sleep(1)

def run():
    args = parse_args()

    if args.debug:
        level = logging.DEBUG
    else:
        level = logging.INFO

    logging.basicConfig(level=level)

    streamer = SerialStreamer(args.port, args.holdoff)
    streamer.start()

    fp = open(args.outfile, 'w')

    try:
        gather(streamer, fp)
    except KeyboardInterrupt:
        logger.info('Terminating')
    finally:
        fp.close()
        streamer.stop()

if __name__ == '__main__':
    run()
