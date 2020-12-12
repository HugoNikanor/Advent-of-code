(use-modules (ice-9 rdelim))

(define directions
  '((#\N . 0+i)
    (#\S . 0-i)
    (#\E . 1+0i)
    (#\W . -1+0i)
    (#\L . 0+i)
    (#\R . 0-i)
    ))

(call-with-values
    (lambda ()
      (let loop ((position 0+0i)
                 (direction 1+0i ))
        (let ((line (read-line)))
          (format #t "position = ~a, direction = ~a~%"
                  position direction)
          (if (eof-object? line)
              (values position direction)
              (let ((cmd (string-ref line 0))
                    (num (string->number (string-drop line 1))))
                (case cmd
                  ((#\F)
                   (loop (+ position (* num direction))
                         direction))
                  ((#\L #\R)
                   (loop position
                         (* direction
                            (expt (assoc-ref directions cmd)
                                  (/ num 90))
                            )))
                  ((#\N #\S #\E #\W)
                   (loop (+ position (* num (assoc-ref directions cmd)))
                         direction))))))))
  (lambda (position direction)
    (format #t "position = ~a, direction = ~a~%"
            position direction)
    (format #t "distance = ~a~%"
            (+ (abs (real-part position))
               (abs (imag-part position))))))
