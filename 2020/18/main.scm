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

(define (main args)
  (define numbers
    (let loop ()
      (let ((line (read-line)))
        (if (eof-object? line)
          '()
          (cons (primitive-eval (lex line))
                (loop))))))


  (display numbers) (newline)
  (display (apply + numbers)) (newline)
  )
