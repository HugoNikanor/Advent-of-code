(use-modules (srfi srfi-41)
             (ice-9 streams)
             (ice-9 match))

(define input (port->stream (current-input-port) read))

;; (format #t "~a (N/A - no previous measurment)~%" (stream-car input))
(define (count-increases input)
  (stream-length
   (stream-filter
    (match-lambda
      ((last this)
       #;
       (format #t "~a (~a)~%"
       this
       (if (> this last)
       "increased"
       "decreased"))
       (> this last)))
    (stream-zip input (stream-cdr input)))))

(format #t "part 1: ~a increases~%" (count-increases input))

(format #t "part 2: ~a increases~%"
 (count-increases
  (stream-map
   +
   input
   (stream-cdr input)
   (stream-cdr (stream-cdr input)))))
