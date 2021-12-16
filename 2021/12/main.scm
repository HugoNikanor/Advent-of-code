(use-modules (srfi srfi-1)
             (ice-9 format)
             (ice-9 rdelim))

(define input
 (let loop ()
   (let ((line (read-line)))
     (if (eof-object? line)
         '()
         (cons (map string->symbol (string-split line #\-))
               (loop))))))

(define graph (make-hash-table))

(for-each (lambda (node)
            (hashq-set! graph (car node)
                        (cons (cadr node) (hashq-ref graph (car node) '())))
            (hashq-set! graph (cadr node)
                        (cons (car node) (hashq-ref graph (cadr node) '()))))
          input)


(define (large? node)
  (char-upper-case? (string-ref (symbol->string node) 0)))

(define (small? node)
  (char-lower-case? (string-ref (symbol->string node) 0)))

(define paths
  (let loop ((position 'start)
             (visited '()))
    (if (eq? 'end position)
        '((end))
        (concatenate
         (map (lambda (x)
                (map (lambda (path) (cons position path))
                     (loop x
                           (if (small? position)
                               (cons position visited)
                               visited))))
              (remove (lambda (x) (memv x visited)) (hashq-ref graph position)))))))


(format #t "~y~%~a paths~%" paths (length paths))
