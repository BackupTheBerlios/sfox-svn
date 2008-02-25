(require 'cc-mode)

;;switch cxx/h
(setq cxxtools-cpp-ext '("cxx" "cpp" "c"))

(defun cxxtools-find-file-buffer (name)
  "Return the buffer conatining the file name"
  (if (null name)
      nil
    (let* ( (buf (get-buffer name)))
      (if (bufferp buf)
          buf
        (condition-case nil
            (find-tag-noselect name) (error nil))))))

(defun cxxtools-scan-list (namelist)
 (let* ( (buf (cxxtools-find-file-buffer (car namelist)) ) )
   (cond
    ( (bufferp buf) buf)
    ( (null (cdr namelist)) '())
    (t (cxxtools-scan-list (cdr namelist))))))

(defun cxxtools-switch-cpp-h ()
 (interactive)
 (let* ( (relname (file-name-nondirectory (buffer-file-name)))
         (nameonly (file-name-sans-extension relname))
         (ext (file-name-extension relname))
         (name-header (concat nameonly ".h"))
         (name-cpp (mapcar '(lambda (x) (concat nameonly "." x)) cxxtools-cpp-ext)) 
	 (buf nil) )
   (if (string-match ext "h")
       (setq buf (cxxtools-scan-list name-cpp))
     (setq buf (cxxtools-find-file-buffer name-header)))
   (if (bufferp buf)
       (switch-to-buffer buf))))

(defun cxxtools-get-include-statement ()
  "Return the string between <> on the current line"
  (save-excursion
    (beginning-of-line)
    (save-match-data
      (if (re-search-forward "<.*>" (line-end-position) t)
          (replace-regexp-in-string "<\\|>" "" (match-string 0))))))

(defun cxxtools-switch-to-included-file ()
  "Switch to the file between <> on the current line"
  (interactive)
  (let ( (include-txt (cxxtools-get-include-statement)) )
    (if (stringp include-txt)
        (let ( (include-buf (cxxtools-find-file-buffer
                             (file-name-nondirectory include-txt))) )
          (if (bufferp include-buf)
              (switch-to-buffer include-buf))))))

;(setq cxxtools-for-regex "\bfor\b\s-*(\s-*\\(.*\\)\s-*;\s-*\\(.*\\)\s-*;\s-*\\(.*\\)\s-*)")
;(setq cxxtools-for-regex "for\W*(\W*\\(.*\\)\W*")
(setq cxxtools-for-regex "\\bfor\\b\\s-*(\\s-*\\(?:\\(.*[^[:blank:]]\\)\\|[:blank:]*\\)\\s-*;\\s-*\\(?:\\(.*[^[:blank:]]\\)\\|[:blank:]*\\)\\s-*;\\s-*\\(?:\\(.*[^[:blank:]]\\)\\|[:blank:]*\\)\\s-*)")
(defun cxxtools-iso ()
  (interactive)
  ;(beginning-of-buffer)
  (move-beginning-of-line nil)
  (if (re-search-forward "\\bfor\\b" (line-end-position) t)
      (let ((for-end (scan-lists (point) 1 0)))
	(backward-char 3)
	(while (re-search-forward cxxtools-for-regex for-end t)
	  (replace-match "for ( \\1; \\2; \\3 )" nil nil)
	  ))))
	
