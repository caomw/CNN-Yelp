import json
import util

f = open("Yelp/yelp_academic_dataset_review.json", "r")
output_f = open("data", "w")
counter = 1
for review in f:
	counter = counter + 1
	#print counter
	parsed = json.loads(review)
	text = parsed["text"]
	text = util.clean_text(text)
        star = parsed["stars"]
        output_f.write(str(star) + "\n")
	output_f.write(text)

f.close()
output_f.close()
