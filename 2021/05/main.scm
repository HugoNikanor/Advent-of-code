(use-modules (ice-9 regex)
             (ice-9 rdelim)
             (ice-9 format)
             (srfi srfi-41)
             (ice-9 streams))




(define lines
  (stream->list
   (stream-map
    (lambda (line)
      (let ((m (string-match "^([0-9]*),([0-9]*) -> ([0-9]*),([0-9]*)$" line)))
        (cons (cons (string->number (match:substring m 1))
                    (string->number (match:substring m 2)))
              (cons (string->number (match:substring m 3))
                    (string->number (match:substring m 4))))))
    (port->stream (current-input-port) read-line))))


(define x car)
(define y cdr)
(define start car)
(define end cdr)
(define start-x (compose x start))
(define start-y (compose y start))
(define end-x (compose x end))
(define end-y (compose y end))

(define straight-lines
  (filter (lambda (line) (or (= (x (start line))
                           (x (end line)))
                        (= (y (start line))
                           (y (end line)))))
          lines))

(define (range from to)
  ((@ (srfi srfi-1) iota)
   (1+ (- (max from to) (min from to))) (min from to)))

(define max-x (1+ (max (apply max (map end-x straight-lines))
                       (apply max (map start-x straight-lines)))))
(define max-y (1+ (max (apply max (map end-y straight-lines))
                       (apply max (map start-y straight-lines)))))

(format #t "max-x = ~a, max-y = ~a~%" max-x max-y)

(define world (make-array 0 max-y max-x))

(format #t "~y" world)

(for-each (lambda (line)
            (format #t "line ~a~%" line)
            (if (= (x (start line))
                   (x (end line)))
                (let ((x (start-x line)))
                  (for-each (lambda (y)
                              (array-set! world (1+ (array-ref world y x))
                                          y x))
                            (range (y (start line))
                                   (y (end line)))))
                (let ((y (start-y line)))
                  (for-each (lambda (x)
                              (array-set! world (1+ (array-ref world y x))
                                          y x))
                            (range (x (start line))
                                   (x (end line)))))))
          straight-lines)


(define count 0)

(for-each (lambda (y)
            (for-each (lambda (x)
                        (when (< 1 (array-ref world y x))
                          (set! count (1+ count))))
                      (iota max-x)))
          (iota max-y))

(if (> 100 max-y)
    (for-each (lambda (y)
                (display #\|)
                (for-each (lambda (x)
                            (let ((value (array-ref world y x)))
                              (if (zero? value)
                                  (format #t " ")
                                  (format #t "~a" value))))
                          (iota max-x))
                (display #\|)
                (newline))
              (iota max-y)))


(format #t "count = ~a~%" count)
