from pydub import AudioSegment

head1 = AudioSegment.from_file("./sourceStems/head1.wav", format="wav")
head2 = AudioSegment.from_file("./sourceStems/head2.wav", format="wav")
top1 = AudioSegment.from_file("./sourceStems/top1.wav", format="wav")
top2 = AudioSegment.from_file("./sourceStems/top2.wav", format="wav")
bottom1 = AudioSegment.from_file("./sourceStems/bottom1.wav", format="wav")
bottom2 = AudioSegment.from_file("./sourceStems/bottom2.wav", format="wav")

headOptions = [[head1, 'head1'], [head2, 'head2'], None]
topOptions = [[top1, 'top1'], [top2, 'top2'], None]
bottomOptions = [[bottom1, 'bottom1'], [bottom2, 'bottom2'], None]


h1t1b1 = head1.overlay(top1, position=0).overlay(bottom1, position=0)

for headOption in headOptions:
    for topOption in topOptions:
        for bottomOption in bottomOptions:
            overlay = None
            if headOption != None:
                if overlay == None:
                    overlay = headOption[0]
                else:
                    overlay = overlay.overlay(headOption[0], position=0)
            if topOption != None:
                if overlay == None:
                    overlay = topOption[0]
                else:
                    overlay = overlay.overlay(topOption[0], position=0)
            if bottomOption != None:
                if overlay == None:
                    overlay = bottomOption[0]
                else:
                    overlay = overlay.overlay(bottomOption[0], position=0)
            if overlay != None:
                overlay.export(f"generated/{(headOption[1] if headOption else "") + (topOption[1] if topOption else "") + (bottomOption[1] if bottomOption else "")}.mp3", format="mp3")
                


# overlay = sound1.overlay(sound2, position=0)


# # simple export
# file_handle = overlay.export("output.mp3", format="mp3")