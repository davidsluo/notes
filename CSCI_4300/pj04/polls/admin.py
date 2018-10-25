from django.contrib import admin

# Register your models here.
from .models import Choice, Question


class ChoiceInLine(admin.TabularInline):
    """
    Allows admin user to add choices to a question.
    """
    model = Choice
    extra = 3


class QuestionAdmin(admin.ModelAdmin):
    """
    Admin model for Questions. Allows admin user to add new questions.
    """
    fieldsets = [
        (None, {'fields': ['question_text']}),
        ('Date information', {'fields': ['pub_date'], 'classes': ['collapse']}),
    ]
    inlines = [ChoiceInLine]
    list_display = ('question_text', 'pub_date', 'was_published_recently')

    list_filter = ['pub_date']
    search_fields = ['question_text']


admin.site.register(Question, QuestionAdmin)
admin.site.register(Choice)
