import json
import util

f = open("Yelp/yelp_academic_dataset_review.json", "r")
output_f = open("words", "w")
#counter = 1
for review in f:
	#counter = counter + 1
	#print counter
	parsed = json.loads(review)
	text = parsed["text"]
	text = util.clean_text(text)
	output_f.write(text)

f.close()
output_f.close()
