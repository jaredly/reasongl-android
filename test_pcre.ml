let () =
  let regexp = Re_pcre.regexp {|\b([a-z]+)\b|} in
  let result = Re.exec regexp "Hello, world!" in
  Format.printf "match: %s\n" (Re.get result 1)