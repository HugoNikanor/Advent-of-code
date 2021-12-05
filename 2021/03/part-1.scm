#!/usr/bin/guile \
-s
!#

(use-modules (srfi srfi-41)
             (srfi srfi-60)
             (ice-9 streams)
             (ice-9 rdelim))

(define bits
 (map positive?
      (map (lambda (l) (apply + l))
           (array->list
            (transpose-array
             (list->array 2
                          (stream->list
                           (stream-map (lambda (s) (map (lambda (c) (case c ((#\0) -1) ((#\1) 1))) (string->list s)))
                                       (port->stream (current-input-port) read-line))))
             1 0)))))

(define gamma (list->integer bits))
(define epsilon (list->integer (map not bits)))

(format #t "~a x ~a = ~a~%" gamma epsilon (* gamma epsilon))
