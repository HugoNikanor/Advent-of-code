#!/usr/bin/guile \
-e main -s
!#

(use-modules ((ice-9 rdelim) :select (read-line)))

(define (lex str)
  (with-input-from-string str
    (lambda ()
      (let loop ((last 'start) (n 0) (m #f) (op #f))
        (let ((c (read-char)))
          (cond ((or (eof-object? c)
                     (char=? c #\))
                     (char=? c #\space))
                 (let ((loop (if (or (eof-object? c) (char=? c #\)))
                               (lambda (_ __ x ___) x)
                               loop)))
                   (case last
                     ((number)
                      (if m
                        (loop 'number 0 `(,op ,n ,m) #f)
                        (loop 'number 0 n op)))
                     ((operator)
                      (loop 'x n m op))
                     ((x)
                      (throw 'parse-error last n m op))

                     #;
                     (if (eof-object? c)
                       (if m
                         (loop 0 `(,op ,n ,m) #f)
                         (loop 0 n op))))))
                ((char-set-contains? char-set:digit c)
                 (loop 'number 
                       (+ (* n 10) (- (char->integer c)
                                      (char->integer #\0)))
                       m op))
                ((char=? c #\()

                 (let ((subexpr (loop 'start 0 #f #f)))
                   (loop 'number
                         subexpr
                         m op))

                 )
                ((memv c '(#\* #\+))
                 (loop 'operator n m (symbol c))
                 )
                (else
                  (throw 'misc-error c)
                  )
                ))))))

(define (lex2 str)
  (with-input-from-string str
    (lambda ()

      (let loop ((n 0))
        (let ((c (read-char)))
          (cond ((or (eof-object? c)
                     (char=? c #\)))
                 (list n))

                ((char=? c #\()
                 (loop (loop 0)))

                ((char=? c #\space)
                 (cons n (loop 0)))

                ((char-set-contains? char-set:digit c)
                 (loop (+ (* n 10)
                          (- (char->integer c)
                             (char->integer #\0)))))

                ((memv c '(#\+ #\*))
                 (loop (symbol c)))))))))

(define (split-by lst delim)
  (reverse
    (let loop ((sublist '())
               (lstlst '())
               (lst lst))
      (cond ((null? lst)
             (cons sublist lstlst))
            ((eqv? delim (car lst))
             (loop '()
                   (cons sublist lstlst)
                   (cdr lst)))
            (else
              (loop (cons (car lst) sublist)
                    lstlst
                    (cdr lst)))))))

(define order-of-operations 
  (reverse '(+ *)))

(define (parse2 tree)
  (if (not (list? tree))
    tree
    (let loop ((ops order-of-operations)
               (tree tree))
      (display ops)
      (display tree) (newline)
      (cond ((not (list? tree)) tree)
            ((null? tree) '(null))
            ((null? ops) 
             (if (list tree)
               (parse2 (car tree))
               tree))
            (else 
              (let ((groups (split-by tree (car ops))))
                (cons (car ops)
                      (map (lambda (sl)
                             (if (= 1 (length sl))
                               (if (not (list? (car sl)))
                                 (car sl)
                                 (parse2 (car sl)))
                               (loop (cdr ops) sl)))
                           groups
                           ))))))))



(define (main args)

  (if (> 2 (length args))
    (display "Usage: ./main (1 | 2) < input\n")
    (let ((part (string->number (cadr args))))

      (define numbers
        (let loop ()
          (let ((line (read-line)))
            (if (eof-object? line)
              '()
              (cons (primitive-eval 
                      (if (= 1 part)
                        (lex line)
                        (parse2 (lex2 line))))
                    (loop))))))


      (display numbers) (newline)
      (display (apply + numbers)) (newline)
      )
  ))
