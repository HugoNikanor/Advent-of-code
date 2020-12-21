#!/usr/bin/guile -s
!#

(import (ice-9 regex)
        (ice-9 rdelim)
        (srfi srfi-1))

(define predicates
  (map (lambda (line)
         (cond ((string-match
                 "^([a-z ]*): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)$"
                 line) => (lambda (m)
                            ;; (display (match:substring m 1)) (newline)
                            (lambda (x)
                              (or (<=
                                   (string->number (match:substring m 2))
                                   x
                                   (string->number (match:substring m 3)))
                                  (<=
                                   (string->number (match:substring m 4))
                                   x
                                   (string->number (match:substring m 5)))))
                            ))
               (else (throw 'match-error))))
       (let loop ()
         (let ((line (read-line)))
           (if (or (eof-object? line)
                   (string-null? line))
               '()
               (cons line (loop)))))))

(read-line)
(define my-ticket (map string->number (string-split (read-line) #\,)))
(read-line)
(read-line)

(define tickets
  (map (lambda (line) (map string->number (string-split line #\,)))
       (let loop ()
         (let ((line (read-line)))
           (if (or (eof-object? line)
                   (string-null? line))
               '()
               (cons line (loop)))))))

(define numbers '())

(map (lambda (ticket)
       ;; (display ticket) (newline)
       (map (lambda (number)
              (unless (fold (lambda (a b) (or a b))
                            #f (map (lambda (p) (p number)) predicates))
                (set! numbers (cons number numbers))))
            ticket))
     tickets)


(display numbers) (newline)
(display (apply + numbers)) (newline)
