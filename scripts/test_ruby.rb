load 'soloud.rb'

begin
    soloud=Soloud.new("")
    speech=Speech.new("")
    soloud.init()
    speech.set_text("Hello ruby world!")
    soloud.play(speech)
    x = 0
    while (soloud.get_active_voice_count() > 0)
        print x, "\r"
        x += 1
    end


ensure
    soloud.deinit()
    speech.destroy()
    soloud.destroy()    
end