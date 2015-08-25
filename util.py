def clean_text(text):
	ret = ""
	space = False
	for c in text:
		if (c >= 'a') and (c <= 'z'):
			ret += c
			space = False
		elif (c >= 'A') and (c <= 'Z'):
			ret += c.lower()
			space = False
		elif (c >= '0') and (c <= '9'):
			ret += c
			space = False
		elif c in ['\'', '-', '_']:
			ret += '_'
			space = False
		else:
			if not space:
				ret += ' '
				space = True
	ret += '\n'
	return ret

	
