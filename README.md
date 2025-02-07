# Atesh or Ate or Atesh 

Part of rewriting this was to write a very basic idea into something that I would use. My first plugin was a very bad and barely functioning distortion plugin; which I would tinker away at between circuit bending pedals for birthday presents. Ates (Fire in turkish... shhh, I'm trying to learn) takes this idea, and leans into it in a unique way, with the key tendents being unstable, volitle and corrupted.

# History

I have a bit of a weird history in creative improvisation, Rock and noise music. For a period of time mixer feedback was my favourite tool; on drum machines, by itself or in the context of a large ensemble. Ideas of managing a choatic system is something that I don't see enough in plugins and personally I would like to see it more. So, with Ates I wanted to lean into this & dilibarately curate a UI that takes my history looking into accessibility and providing something that I find fun to use.

# Wait you added Accessibility?

So, a lot of the accessibility is handled by JUCE under the hood and tbh keyboard navigation can be super helpful even for people who are fully sighted using a mouse. Although, I am not Visually Impaired and am totally open to any feedback on UI/UX changes. At the moment, there is no value input and no one but me has used this.

# UI Design (Why so ugly?)

I will be the first to tell you that I do not have an eye for Software UI design, and at the moment I don't have the time learn.

# Building

The build system is projucer based with Juce as a submodule & when I get time I will properly integrate full cmake support.

# Build Sets

- Clone this repo
- make sure to init the Juce submodule
- build the projucer out of this JUCE submodule
- Open the projucer file for this project.

# Notes

I have only built this on macOS and it hasn't been through any extensive human or animal testing.... So, if you decide to try this and find anything please create an issue and i will take a look.

