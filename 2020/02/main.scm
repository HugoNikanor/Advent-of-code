#!/usr/bin/guile -s
!#

(use-modules (ice-9 rdelim)
             (ice-9 regex)
             )

(define f (open-input-file "input"))

(define good 0)

(let loop ()
  (let ((line (read-line f)))
    (unless (eof-object? line)
      (let ((m (string-match "([0-9]+)-([0-9]+) (.): (.*)" line)))
        (let ((min (string->number (match:substring m 1)))
              (max (string->number (match:substring m 2)))
              (char (string-ref (match:substring m 3) 0))
              (pass (match:substring m 4)))
          (when (<= min (string-count pass char) max)
            (set! good (1+ good)))
          (loop)
          )))))


(format #t "good = ~a~%" good)
