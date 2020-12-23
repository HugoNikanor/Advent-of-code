
(define min-x 0)
(define max-x 0)
(define min-y 0)
(define max-y 0)
(define min-z 0)
(define max-z 0)

(define world
  (let ((dim (+ 8 (* 6 2) 1)))
    (make-array 0 3 3 3)))

(for-each (lambda (y)
            (let ((line (read-line)))
              (map (lambda (c x)
                     (array-set! world
                                 (+ x 6) (+ y 6) 10
                                 (if (char=? c #\#) 1 0))
                     (string->list line)
                     (iota (string-length line))))))
          (iota 8))

(define active-states '((3 0 0)
                        (7 0 0)
                        (2 1 0)
                        ;; ...
                        ))

(for cube in active-state
     (count alive? (neighbours world cube))
