#ifndef ELIZA_TEMPLATES_H
#define ELIZA_TEMPLATES_H

#include <stddef.h>

static const char *emotion_words[] = {
    "sad", "depressed", "angry", "mad", "furious",
    "happy", "glad", "joyful", "anxious", "nervous",
    "scared", "afraid", "fearful", "lonely", "alone",
    "confused", "frustrated", "worried", "guilty",
    "ashamed", "hopeful", "excited", "hurt", "betrayed",
    "jealous", "embarrassed", "overwhelmed", "stressed",
    "tired", "exhausted", "hopeless", "empty",
    "worthless", "useless", "stupid", "pathetic",
    "failure", "inadequate", "insecure",
    "miserable", "horrible", "terrible", "awful",
    "sick", "crappy", "shitty",
    "ugly", "hideous", "disgusting", "repulsive"
};
#define N_EMOTIONS (sizeof(emotion_words)/sizeof(emotion_words[0]))

static const char *family_words[] = {
    "mother", "father", "mom", "dad", "parent",
    "sister", "brother", "wife", "husband",
    "aunt", "uncle", "grandma", "grandpa",
    "grandmother", "grandfather", "family",
    "child", "children", "daughter", "son"
};
#define N_FAMILY (sizeof(family_words)/sizeof(family_words[0]))

static const char *absolutist_words[] = {
    "always", "never", "everyone", "no one", "nobody", "everybody"
};
#define N_ABSOLUTIST (sizeof(absolutist_words)/sizeof(absolutist_words[0]))

static const char *negation_words[] = {
    "no", "not", "never", "don't", "can't", "cannot",
    "won't", "doesn't", "didn't", "isn't", "aren't"
};
#define N_NEGATION (sizeof(negation_words)/sizeof(negation_words[0]))

static const char *agreement_words[] = {
    "yes", "yeah", "yep", "okay", "sure", "alright", "fine"
};
#define N_AGREEMENT (sizeof(agreement_words)/sizeof(agreement_words[0]))

static const char *help_words[] = {
    "help", "need", "please", "can you"
};
#define N_HELP (sizeof(help_words)/sizeof(help_words[0]))

static const char *relationship_words[] = {
    "friend", "relationship", "boyfriend", "girlfriend",
    "partner", "marriage", "divorce", "breakup",
    "dating", "sex", "cheating", "trust",
    "ex", "crush", "love"
};
#define N_RELATIONSHIP (sizeof(relationship_words)/sizeof(relationship_words[0]))

static const char *feeling_phrases[] = {
    "i feel", "i am feeling", "i'm feeling",
    "i have been feeling", "i've been feeling",
    "i am", "i'm"
};
#define N_FEELING_PHRASES (sizeof(feeling_phrases)/sizeof(feeling_phrases[0]))

static const char *statement_phrases[] = {
    "i think", "i believe", "i want", "i need",
    "i wish", "i hope", "i know", "i wonder"
};
#define N_STATEMENT_PHRASES (sizeof(statement_phrases)/sizeof(statement_phrases[0]))

static const char *dream_words[] = {
    "dream", "nightmare", "dreamt", "dreamed", "dreaming"
};
#define N_DREAM (sizeof(dream_words)/sizeof(dream_words[0]))

static const char *because_words[] = {
    "because", "reason", "since", "as"
};
#define N_BECAUSE (sizeof(because_words)/sizeof(because_words[0]))

static const char *strong_emotion_words[] = {
    "hate", "love", "despise", "adore", "loathe",
    "bitch", "bastard", "asshole",
    "fuck", "fucking", "shit", "damn"
};
#define N_STRONG_EMOTION (sizeof(strong_emotion_words)/sizeof(strong_emotion_words[0]))

static const char *sorry_words[] = {
    "sorry", "apologize", "apology", "forgive"
};
#define N_SORRY (sizeof(sorry_words)/sizeof(sorry_words[0]))

static const char *work_school_words[] = {
    "job", "work", "boss", "coworker", "career",
    "fired", "hired", "promotion", "salary",
    "school", "teacher", "student", "class", "homework",
    "exam", "college", "university", "major", "degree",
    "grades", "study", "studying", "office", "shift"
};
#define N_WORK_SCHOOL (sizeof(work_school_words)/sizeof(work_school_words[0]))

static const char *health_body_words[] = {
    "health", "sick", "illness", "disease", "pain",
    "doctor", "hospital", "sleep", "insomnia",
    "weight", "body", "exercise", "medicine",
    "medication", "surgery", "therapy", "chronic",
    "headache", "back pain", "cancer", "heart",
    "anxiety attack", "panic attack"
};
#define N_HEALTH_BODY (sizeof(health_body_words)/sizeof(health_body_words[0]))

static const char *future_words[] = {
    "future", "tomorrow", "next week", "next year",
    "plan", "goal", "hoping", "wondering",
    "soon", "eventually", "someday"
};
#define N_FUTURE (sizeof(future_words)/sizeof(future_words[0]))

static const char *gratitude_words[] = {
    "thanks", "thank you", "appreciate", "grateful",
    "blessed", "thankful"
};
#define N_GRATITUDE (sizeof(gratitude_words)/sizeof(gratitude_words[0]))

static const char *existential_words[] = {
    "meaning", "purpose", "exist", "existence",
    "universe", "god", "afterlife", "reality",
    "conscious", "fate", "soul", "spiritual",
    "universe", "why are we", "point of life"
};
#define N_EXISTENTIAL (sizeof(existential_words)/sizeof(existential_words[0]))

static const char *trauma_words[] = {
    "trauma", "abuse", "assaulted", "violence",
    "war", "accident", "disaster", "crash",
    "abusive", "traumatic", "survivor"
};
#define N_TRAUMA (sizeof(trauma_words)/sizeof(trauma_words[0]))

static const char *silence_words[] = {
    "idk", "dunno", "hmm", "umm", "uh", "nah", "yeah", "k"
};
#define N_SILENCE (sizeof(silence_words)/sizeof(silence_words[0]))

static const char *silence_responses[] = {
    "It's okay. Take your time.",
    "No rush. I'm here when you're ready.",
    "You seem unsure. That's alright.",
    "Sometimes it's hard to find the words.",
    "I'm listening. Go at your own pace.",
    "You can tell me anything that's on your mind.",
};
#define N_SILENCE_R (sizeof(silence_responses)/sizeof(silence_responses[0]))

static const char *crisis_phrases[] = {
    "suicide", "kill myself", "end my life", "want to die",
    "hurt myself", "self-harm", "cutting", "not worth living",
    "better off dead", "end it all", "take my own life",
    "don't want to live", "wish I was dead",
    "death", "dying", "died",
};
#define N_CRISIS (sizeof(crisis_phrases)/sizeof(crisis_phrases[0]))

static const char *crisis_templates[] = {
    "I'm really glad you told me that. Can you tell me more about how you're feeling?",
    "That sounds really heavy. You're not alone in this. What's been going on?",
    "I hear you. Those feelings are important to talk about. Can you tell me more?",
    "Thank you for trusting me with that. How long have you been feeling this way?",
    "That's a lot to carry. What do you think led to these feelings?",
    "I'm here with you. What's been going through your mind?",
    "Tell me more about what's bringing these feelings up.",
};
#define N_CRISIS_T (sizeof(crisis_templates)/sizeof(crisis_templates[0]))

static const char *greeting_templates[] = {
    "Hello. How are you feeling today?",
    "Hi there. What brings you here?",
    "Good to see you. How can I help?",
    "Hey. Tell me what's on your mind.",
    "Welcome. I'm here to listen.",
    "Hello. What would you like to talk about today?",
    "Hi. How are things going?",
};
#define N_GREETING_T (sizeof(greeting_templates)/sizeof(greeting_templates[0]))

static const char *reflect_feeling_templates[] = {
    "[REFLECTED].",
    "You feel [EMOTION]? Tell me more about that.",
    "Why do you feel [EMOTION]?",
    "When did you start feeling [EMOTION]?",
    "How long have you been feeling [EMOTION]?",
    "What does feeling [EMOTION] make you think about?",
    "Tell me more about what [EMOTION] feels like for you.",
    "Can you describe what [EMOTION] feels like?",
    "What do you think is at the root of feeling [EMOTION]?",
    "Is there a specific reason you're feeling [EMOTION] today?",
    "Do you remember when you started feeling [EMOTION]?",
    "[REFLECTED]? Tell me more.",
};
#define N_REFLECT_FEELING_T (sizeof(reflect_feeling_templates)/sizeof(reflect_feeling_templates[0]))

static const char *reflect_statement_templates[] = {
    "[REFLECTED]?",
    "What makes you say that?",
    "Can you elaborate on that?",
    "How does that make you feel?",
    "I understand. What else comes to mind?",
    "Tell me more about what you mean by that.",
    "That's interesting. Why do you think that is?",
    "What led you to think about that?",
    "How long have you thought that way?",
    "Has something happened recently to make you feel this way?",
};
#define N_REFLECT_STATEMENT_T (sizeof(reflect_statement_templates)/sizeof(reflect_statement_templates[0]))

static const char *question_open_templates[] = {
    "[REFLECTED]?",
    "What does that mean to you?",
    "How does that make you feel?",
    "Can you tell me more about that?",
    "What comes to mind when you think about that?",
    "That's a good question. What do you think the answer might be?",
    "What do you hope will happen?",
    "If you knew the answer, what would it be?",
    "What's the first thing that comes to mind when you ask that?",
    "That's an important question. What do you think?",
};
#define N_QUESTION_OPEN_T (sizeof(question_open_templates)/sizeof(question_open_templates[0]))

static const char *question_feeling_templates[] = {
    "[REFLECTED]?",
    "When did you start feeling [EMOTION]?",
    "How does feeling [EMOTION] affect you?",
    "What do you think is causing you to feel [EMOTION]?",
    "Does feeling [EMOTION] remind you of anything?",
    "What happens in your life when you feel [EMOTION]?",
    "Has there been a time you didn't feel [EMOTION]?",
};
#define N_QUESTION_FEELING_T (sizeof(question_feeling_templates)/sizeof(question_feeling_templates[0]))

static const char *encourage_templates[] = {
    "It sounds like you're going through a difficult time.",
    "That must be hard for you.",
    "I appreciate you sharing that with me.",
    "It takes courage to talk about these things.",
    "I understand. Please go on.",
    "That sounds really challenging. I'm glad you're talking about it.",
    "It's okay to feel that way. Tell me more.",
    "You're not alone in feeling this way.",
    "Thank you for opening up about that.",
    "That's a lot to deal with. How are you holding up?",
    "I hear you. Keep going.",
};
#define N_ENCOURAGE_T (sizeof(encourage_templates)/sizeof(encourage_templates[0]))

static const char *family_templates[] = {
    "Tell me more about your [FAMILY].",
    "How does your relationship with your [FAMILY] affect you?",
    "What comes to mind when you think of your [FAMILY]?",
    "It seems like your [FAMILY] is important to you.",
    "How do you feel about your [FAMILY]?",
    "What was your childhood like with your [FAMILY]?",
    "Do you feel close to your [FAMILY]?",
    "How has your relationship with your [FAMILY] changed over time?",
    "What do you wish was different about your [FAMILY]?",
};
#define N_FAMILY_T (sizeof(family_templates)/sizeof(family_templates[0]))

static const char *dream_templates[] = {
    "What do you think your dream means?",
    "Dreams can be very revealing. What comes to mind?",
    "How did that dream make you feel?",
    "Do you often have dreams like that?",
    "What do you associate with that dream?",
    "That's an interesting dream. What stands out about it?",
    "Do you think the dream relates to something in your life?",
    "How do you feel when you wake up from that dream?",
};
#define N_DREAM_T (sizeof(dream_templates)/sizeof(dream_templates[0]))

static const char *transference_templates[] = {
    "Why do you say that?",
    "How does that relate to how you're feeling?",
    "What makes you feel that way about me?",
    "Let's talk more about you.",
    "You seem to be focusing on me. What's on your mind?",
    "It sounds like you have strong feelings about this.",
    "Tell me what's really going on.",
    "I'm here to listen. What do you want to talk about?",
};
#define N_TRANSPERENCE_T (sizeof(transference_templates)/sizeof(transference_templates[0]))

static const char *work_school_templates[] = {
    "Tell me about your work.",
    "How does your job make you feel?",
    "Work can be stressful. What's going on?",
    "It sounds like work has been difficult.",
    "How do you feel about your career?",
    "What's the hardest part about your job right now?",
    "Tell me more about your situation at work.",
    "How long has this been going on at work?",
    "Does your job affect how you feel otherwise?",
    "What would you change about your work life?",
    "Work stress can bleed into everything. Tell me more.",
    "How do you cope with work stress?",
};
#define N_WORK_SCHOOL_T (sizeof(work_school_templates)/sizeof(work_school_templates[0]))

static const char *health_body_templates[] = {
    "Tell me about your health concerns.",
    "How does your health affect your daily life?",
    "Have you spoken to a doctor about this?",
    "That sounds difficult to deal with.",
    "Health problems can be really draining.",
    "How long have you been dealing with this?",
    "Does your health affect your mood?",
    "I'm sorry you're going through that.",
    "What's the hardest part about your health situation?",
    "How do you cope with your health issues?",
    "It must be tough managing all of that.",
    "How does your physical health connect to how you feel mentally?",
};
#define N_HEALTH_BODY_T (sizeof(health_body_templates)/sizeof(health_body_templates[0]))

static const char *self_esteem_templates[] = {
    "Why do you feel that way about yourself?",
    "You're being hard on yourself. What makes you say that?",
    "Have you always felt this way about yourself?",
    "What would say to a friend who felt this way?",
    "What makes you feel that you're not good enough?",
    "Tell me more about why you feel that way.",
    "Do you often compare yourself to others?",
    "What would need to change for you to feel better about yourself?",
    "Where do you think these feelings about yourself come from?",
    "You're talking to yourself harshly. What's behind that?",
};
#define N_SELF_ESTEEM_T (sizeof(self_esteem_templates)/sizeof(self_esteem_templates[0]))

static const char *existential_templates[] = {
    "That's a deep question. What do you think?",
    "It sounds like you're searching for meaning.",
    "What does a meaningful life look like to you?",
    "That's something many people think about.",
    "What makes you feel that life has purpose?",
    "Have you felt this way for long?",
    "What do you think gives life meaning?",
    "Those are important questions to ask yourself.",
    "What experiences have shaped your view on that?",
    "Not everyone asks those questions. What brought them up for you?",
};
#define N_EXISTENTIAL_T (sizeof(existential_templates)/sizeof(existential_templates[0]))

static const char *default_templates[] = {
    "Tell me more about that.",
    "I see. Please continue.",
    "That's interesting. Can you tell me more?",
    "How does that make you feel?",
    "What else is on your mind?",
    "I'm listening.",
    "Go on.",
    "Help me understand better.",
    "What do you mean by that?",
    "Can you tell me more about what's going on?",
    "I'd like to hear more about that.",
    "Keep going. I'm here.",
};
#define N_DEFAULT_T (sizeof(default_templates)/sizeof(default_templates[0]))

static const char *fallback_pool[] = {
    "Tell me more about that.",
    "I see. Please continue.",
    "That's interesting. Can you elaborate?",
    "How does that make you feel?",
    "What else comes to mind?",
    "I'm listening.",
    "Go on.",
    "I understand. Keep going.",
};
#define N_FALLBACK (sizeof(fallback_pool)/sizeof(fallback_pool[0]))

#endif