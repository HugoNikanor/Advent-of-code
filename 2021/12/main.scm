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
             (used-token? #f)
             (visited '(start)))
    (if (eq? 'end position)
        '((end))
        (if used-token?
            (concatenate
             (map (lambda (x)
                    (map (lambda (path) (cons position path))
                         (loop x used-token?
                               (if (small? x)
                                   (cons x visited)
                                   visited))))
                  (remove (lambda (x) (memv x visited)) (hashq-ref graph position))))
            ;; else
            (append
             ;; don't use token
             (concatenate
              (map (lambda (x)
                     (map (lambda (path) (cons position path))
                          (loop x used-token?
                                (if (small? x)
                                    (cons x visited)
                                    visited))))
                   (remove (lambda (x) (memv x visited)) (hashq-ref graph position))))
             ;; use token
             (concatenate
              (map (lambda (x)
                     (map (lambda (path) (cons position path))
                          (loop x #t visited)))
                   (remove (lambda (x) (memv x visited)) (hashq-ref graph position)))))

            ))))


;; (format #t "~y~%~a paths~%" paths (length paths))
(for-each (lambda (path)
            (format #t "~a~%" (string-join (map symbol->string path) ",")))
          paths)
