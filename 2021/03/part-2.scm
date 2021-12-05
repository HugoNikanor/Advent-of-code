#!/usr/bin/guile \
-s
!#

(use-modules (srfi srfi-41)
             (srfi srfi-60)
             (ice-9 streams)
             (ice-9 rdelim))

;; (define life-support-rating
;;   (* oxygen-generator-rating
;;      CO2-scrubber-rating))

(define bits
  (stream->list
    (stream-map (lambda (s) (map (lambda (c) (case c ((#\0) -1) ((#\1) 1))) (string->list s)))
                (port->stream (current-input-port) read-line))))

(define (find-rating extra-proc bits)
  (let loop ((idx 0) (bitset bits))
    (if (>= 1 (length bitset))
      (car bitset)
      (let* ((sum (apply + (map (lambda (b) (list-ref b idx)) bitset)))
             (target-bit
               (extra-proc (cond ((> sum 0) 1)
                                 ((< sum 0) -1)
                                 ((= sum 0) 1)))))
        (loop (1+ idx)
              (filter (lambda (b) (= target-bit (list-ref b idx)))
                      bitset))))))

(define oxygen-generator-rating (list->integer (map positive? (find-rating + bits))))
(define CO2-scrubber-rating     (list->integer (map positive? (find-rating - bits))))

(format #t "~a x ~a = ~a~%"
        oxygen-generator-rating
        CO2-scrubber-rating
        (* oxygen-generator-rating
           CO2-scrubber-rating))




;;; for every number
;;;     for each bit from the left of each number
;;;       if the number matches the bit criteria
;;;           keep the number
;;;       else
;;;           skip the number
;;;       if this is the last number in the set
;;;           return it
;;; 
;;; 
;;; oxygen-generator-ratnig bit criteria
;;;     find the most common value for a given bit
;;;     ties goes to 1
;;;     all numbers with that number on that position should be kept
;;; 
;;; CO2-scrubber-rating bit criteria
;;;     not oxygen-generator-ratnig bit criteria
